/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha224_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:44:50 by bbrassar          #+#    #+#             */
/*   Updated: 2025/06/02 12:40:50 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash/sha2.h"
#include "hash/sha256.c"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define sizeof_array(Arr) (sizeof(Arr) / sizeof((Arr)[0]))

struct sha256_test {
	char const *input;
	char const digest[SHA224_DIGEST_SIZE * 2 + 1];
};

static struct sha256_test const TESTS[] = {
	{
		.input = "",
		.digest =
			"d14a028c2a3a2bc9476102bb288234c415a2b01f828ea62ac5b3e42f",
	},
	{
		.input = "1",
		.digest =
			"e25388fde8290dc286a6164fa2d97e551b53498dcbf7bc378eb1f178",
	},
	{
		.input = "0000000000000000", // 16
		.digest =
			"e22fca9b8a76667e2465ba3978ca25393a3eb7edc0dccedf45231e90",
	},
	{
		.input = "00000000000000000000000000000000", // 32
		.digest =
			"64261be56d8894db5d16eb6bbdb6be1309d52ed18ef4f3bcadbb45bb",
	},
	{
		.input =
			"000000000000000000000000000000000000000000000000", // 48
		.digest =
			"799f64099004c75b3015d835db7eb74959fb05b85dc76c7e1094819a",
	},
	{
		.input =
			"0000000000000000000000000000000000000000000000000000000000000000", // 64
		.digest =
			"fc5d6aed7146d6747dd6fca075f9fe5a30a4c0c9ff67effc484f10b5",
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

static int test_sha224(struct sha256_test const *test)
{
	struct sha256_context ctx;
	uint8_t digest[SHA224_DIGEST_SIZE];
	char digest_s[SHA224_DIGEST_SIZE * 2 + 1];

	sha224_init(&ctx);

	sha256_update(&ctx, test->input, strlen(test->input));
	sha224_digest(&ctx, digest);

	for (int i = 0; i < SHA224_DIGEST_SIZE; i += 1) {
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
		int result = test_sha224(&TESTS[i]);

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
