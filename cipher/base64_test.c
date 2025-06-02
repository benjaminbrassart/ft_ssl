/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:08:07 by bbrassar          #+#    #+#             */
/*   Updated: 2025/06/02 17:27:50 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cipher/base64.c"
#include "cipher/base64.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define sizeof_array(Arr) (sizeof(Arr) / sizeof(*Arr))

struct base64_block_test {
	uint8_t const decoded[3];
	char const encoded[4];
};

static struct base64_block_test const BLOCK_TESTS[] = {
	{
		.decoded = { 0x00, 0x00, 0x00 },
		.encoded = { 'A', 'A', 'A', 'A' },
	},
	{
		.decoded = { 0xff, 0xff, 0xff },
		.encoded = { '/', '/', '/', '/' },
	},
	{
		.decoded = { 'M', 'a', 'n' },
		.encoded = { 'T', 'W', 'F', 'u' },
	},
};

static int test_encode_block(struct base64_block_test const *test)
{
	char out[4];

	encode_full_block(test->decoded, out);
	if (memcmp(out, test->encoded, 4) != 0) {
		fprintf(stderr, "Encode block test failed:\n");
		fprintf(stderr, "  Input: %02x %02x %02x\n", test->decoded[0],
			test->decoded[1], test->decoded[2]);
		fprintf(stderr, "\n");
		fprintf(stderr, "  Want: \"%.4s\"\n", test->encoded);
		fprintf(stderr, "  Have: \"%.4s\"\n", out);
		fprintf(stderr, "\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

static int test_decode_block(struct base64_block_test const *test)
{
	uint8_t out[3];
	int res = decode_full_block(test->encoded, out);

	if (res != EXIT_SUCCESS || memcmp(out, test->decoded, 3) != 0) {
		fprintf(stderr, "Decode block test failed:\n");
		fprintf(stderr, "  Input: \"%.4s\"\n", test->encoded);
		fprintf(stderr, "\n");
		fprintf(stderr, "  Want: %02x %02x %02x\n", test->decoded[0],
			test->decoded[1], test->decoded[2]);
		fprintf(stderr, "  Have: %02x %02x %02x\n", out[0], out[1],
			out[2]);
		fprintf(stderr, "  Return code: %d\n", res);
		fprintf(stderr, "\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

struct base64_stream_test {
	uint8_t const *decoded;
	size_t decoded_length;
	char const *encoded;
	char const *encoded_padded;
};

static struct base64_stream_test const STREAM_TESTS[] = {
	{
		.decoded = (uint8_t const *)"",
		.decoded_length = 0,
		.encoded = "",
		.encoded_padded = "",
	},
	{
		.decoded = (uint8_t const *)"M",
		.decoded_length = 1,
		.encoded = "TQ",
		.encoded_padded = "TQ==",
	},
	{
		.decoded = (uint8_t const *)"Ma",
		.decoded_length = 2,
		.encoded = "TWE",
		.encoded_padded = "TWE=",
	},
	{
		.decoded = (uint8_t const *)"Man",
		.decoded_length = 3,
		.encoded = "TWFu",
		.encoded_padded = "TWFu",
	},
};

struct enc_test {
	char *buffer;
	size_t buffer_len;
};

static int encode_callback(void *ctx, char const encoded[4])
{
	struct enc_test *buf = ctx;
	size_t const len = strnlen(encoded, 4);
	size_t const new_len = len + buf->buffer_len;
	char *const new_buffer = realloc(buf->buffer, new_len + 1);

	if (new_buffer == NULL) {
		return EXIT_FAILURE;
	}

	memcpy(&new_buffer[buf->buffer_len], encoded, len);
	new_buffer[new_len] = '\0';

	buf->buffer = new_buffer;
	buf->buffer_len = new_len;

	return EXIT_SUCCESS;
}

static int test_encode_stream(struct base64_stream_test const *test)
{
	int res;

	{
		struct enc_test buf = { NULL, 0 };
		struct base64_encoder enc;

		base64_enc_init(&enc, &buf, encode_callback);
		res = base64_enc_update(&enc, test->decoded,
					test->decoded_length);
		if (res != EXIT_SUCCESS) {
			return res;
		}

		res = base64_enc_finalize(&enc, 0);
		if (res != EXIT_SUCCESS) {
			return res;
		}

		if (strcmp(test->encoded, buf.buffer) != 0) {
			return EXIT_FAILURE;
		}

		free(buf.buffer);
	}

	{
		struct enc_test buf = { NULL, 0 };
		struct base64_encoder enc;

		base64_enc_init(&enc, &buf, encode_callback);
		res = base64_enc_update(&enc, test->decoded,
					test->decoded_length);
		if (res != EXIT_SUCCESS) {
			return res;
		}

		res = base64_enc_finalize(&enc, 1);
		if (res != EXIT_SUCCESS) {
			return res;
		}

		if (strcmp(test->encoded_padded, buf.buffer) != 0) {
			return EXIT_FAILURE;
		}

		free(buf.buffer);
	}

	return EXIT_FAILURE;
}

static int test_decode_stream(struct base64_stream_test const *test)
{
	(void)test;
	return EXIT_FAILURE;
}

int main(void)
{
	for (size_t i = 0; i < sizeof_array(BLOCK_TESTS); i += 1) {
		test_encode_block(&BLOCK_TESTS[i]);
		test_decode_block(&BLOCK_TESTS[i]);
	}

	for (size_t i = 0; i < sizeof_array(STREAM_TESTS); i += 1) {
		test_encode_stream(&STREAM_TESTS[i]);
		test_decode_stream(&STREAM_TESTS[i]);
	}
}
