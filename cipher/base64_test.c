/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 13:08:07 by bbrassar          #+#    #+#             */
/*   Updated: 2025/06/02 14:56:03 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cipher/base64.c"
#include "cipher/base64.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define sizeof_array(Arr) (sizeof(Arr) / sizeof(*Arr))

struct base64_test {
	uint8_t const decoded[3];
	char const encoded[4];
};

static struct base64_test const BLOCK_TESTS[] = {
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

static int test_encode_block(struct base64_test const *test)
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

static int test_decode_block(struct base64_test const *test)
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

int main(void)
{
	for (size_t i = 0; i < sizeof_array(BLOCK_TESTS); i += 1) {
		test_encode_block(&BLOCK_TESTS[i]);
		test_decode_block(&BLOCK_TESTS[i]);
	}
}
