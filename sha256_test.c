/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:05:28 by bbrassar          #+#    #+#             */
/*   Updated: 2025/05/30 11:08:47 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sha256.c"
#include "sha256.h"

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
			"4bf5122f344554c53bde2ebb8cd2b7e3d1600ad631c385a5d7cce23c7785459a",
	},
	{
		.input = "0000000000000000", // 16
		.digest =
			"af5570f5a1810b7af78caf4bc70a660f0df51e42baf91d4de5b2328de0e83dfc",
	},
	{
		.input = "00000000000000000000000000000000", // 32
		.digest =
			"374708fff7719dd5979ec875d56cd2286f6d3cf7ec317a3b25632aab28ec37bb",
	},
	{
		.input =
			"000000000000000000000000000000000000000000000000", // 48
		.digest =
			"9d908ecfb6b256def8b49a7c504e6c889c4b0e41fe6ce3e01863dd7b61a20aa0",
	},
	{
		.input =
			"0000000000000000000000000000000000000000000000000000000000000000", // 64
		.digest =
			"66687aadf862bd776c8fc18b8e9f8e20089714856ee233b3902a591d0d5f2925",
	},
	{
		.input = "The quick brown fox jumps over the lazy dog",
		.digest =
			"730e109bd7a8a32b1cb9d9a09aa2325d2430587ddbc0c38bad911525",
	},
	{
		.input = "The quick brown fox jumps over the lazy dog.",
		.digest =
			"619cba8e8e05826e9b8c519c0a5c68f4fb653e8a3d8aa04bb2c8cd4c",
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
