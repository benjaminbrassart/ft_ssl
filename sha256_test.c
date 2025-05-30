/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:05:28 by bbrassar          #+#    #+#             */
/*   Updated: 2025/05/30 11:39:23 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sha2.h"
#include "sha256.c"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 64
#define sizeof_array(Arr) (sizeof(Arr) / sizeof((Arr)[0]))

struct sha256_test {
	char const *input;
	char const digest[SHA256_DIGEST_SIZE * 2 + 1];
};

static struct sha256_test const TESTS[] = {
	{
		.input = "",
		.digest =
			"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855",
	},
	{
		.input = "1",
		.digest =
			"6b86b273ff34fce19d6b804eff5a3f5747ada4eaa22f1d49c01e52ddb7875b4b",
	},
	{
		.input = "0000000000000000", // 16
		.digest =
			"fcdb4b423f4e5283afa249d762ef6aef150e91fccd810d43e5e719d14512dec7",
	},
	{
		.input = "00000000000000000000000000000000", // 32
		.digest =
			"84e0c0eafaa95a34c293f278ac52e45ce537bab5e752a00e6959a13ae103b65a",
	},
	{
		.input =
			"000000000000000000000000000000000000000000000000", // 48
		.digest =
			"f9a2ba511957122bfa67b029061c679703494540b35f02e0e0496a3b0cdcc46a",
	},
	{
		.input =
			"0000000000000000000000000000000000000000000000000000000000000000", // 64
		.digest =
			"60e05bd1b195af2f94112fa7197a5c88289058840ce7c6df9693756bc6250f55",
	},
	{
		.input = "The quick brown fox jumps over the lazy dog",
		.digest =
			"d7a8fbb307d7809469ca9abcb0082e4f8d5651e46d3cdb762d02d0bf37c9e592",
	},
	{
		.input = "The quick brown fox jumps over the lazy dog.",
		.digest =
			"ef537f25c895bfa782526529a9b63d97aa631564d5d789c2b765448c8635fb6c",
	},
};

static int test_sha256(struct sha256_test const *test)
{
	struct sha256_context ctx;
	uint8_t digest[SHA256_DIGEST_SIZE];
	char digest_s[SHA256_DIGEST_SIZE * 2 + 1];

	sha256_init(&ctx);

	sha256_update(&ctx, test->input, strlen(test->input));
	sha256_digest(&ctx, digest);

	for (int i = 0; i < SHA256_DIGEST_SIZE; i += 1) {
		sprintf(&digest_s[i * 2], "%02x", digest[i]);
	}

	if (strcmp(digest_s, test->digest) == 0) {
		return EXIT_SUCCESS;
	}

	fprintf(stderr, "==================================================\n");
	fprintf(stderr, "  Input: \"%s\"\n", test->input);
	fprintf(stderr, "  Want : %s\n", test->digest);
	fprintf(stderr, "  Have : %s\n", digest_s);
	fprintf(stderr, "==================================================\n");

	return EXIT_FAILURE;
}

int main(void)
{
	int ok = 0;
	int ko = 0;

	for (size_t i = 0; i < sizeof_array(TESTS); i += 1) {
		int result = test_sha256(&TESTS[i]);

		if (result != EXIT_SUCCESS) {
			ko += 1;
		} else {
			ok += 1;
		}
	}

	if (ko > 0) {
		fprintf(stderr, "\n-- TESTS FAILED (%d OK, %d KO)\n", ok, ko);
	}

	return ko > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
