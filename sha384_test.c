/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha384_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 12:18:23 by bbrassar          #+#    #+#             */
/*   Updated: 2025/05/30 12:27:14 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sha2.h"
#include "sha512.c"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define sizeof_array(Arr) (sizeof(Arr) / sizeof((Arr)[0]))

struct sha384_test {
	char const *input;
	char const digest[SHA384_DIGEST_SIZE * 2 + 1];
};

static struct sha384_test const TESTS[] = {
	{
		.input = "",
		.digest =
			"38b060a751ac96384cd9327eb1b1e36a21fdb71114be07434c0cc7bf63f6e1da274edebfe76f65fbd51ad2f14898b95b",
	},
	{
		.input = "1",
		.digest =
			"47f05d367b0c32e438fb63e6cf4a5f35c2aa2f90dc7543f8a41a0f95ce8a40a313ab5cf36134a2068c4c969cb50db776",
	},
	{
		.input = "0000000000000000", // 16
		.digest =
			"313451ea0161b866f14363ac40f83c978a257f99c22dc2bad084d1748c04dc4275880d2e1e8adabefdef72233498a031",
	},
	{
		.input = "00000000000000000000000000000000", // 32
		.digest =
			"c4be759d6ed1defbfc19b28721759e65495bfc7f97e46f3e4b65a803b7e1cf46a891dbf20b1ca41d006c9b9e3c106537",
	},
	{
		.input =
			"000000000000000000000000000000000000000000000000", // 48
		.digest =
			"6cb7102c1a22a11921d4a7b0d08214836223e5ac987802166a94b071e580ed4f638a903633334f6d88010b8f49585fb9",
	},
	{
		.input =
			"0000000000000000000000000000000000000000000000000000000000000000", // 64
		.digest =
			"53871b26a08e90cb62142f2a39f0b80de41792322b0ca5602b6eb7b5cf067c49498a7492bb9364bbf90f40c1c5412105",
	},
	{
		.input = "The quick brown fox jumps over the lazy dog",
		.digest =
			"ca737f1014a48f4c0b6dd43cb177b0afd9e5169367544c494011e3317dbf9a509cb1e5dc1e85a941bbee3d7f2afbc9b1",
	},
	{
		.input = "The quick brown fox jumps over the lazy dog.",
		.digest =
			"ed892481d8272ca6df370bf706e4d7bc1b5739fa2177aae6c50e946678718fc67a7af2819a021c2fc34e91bdb63409d7",
	},
};

static int test_sha384(struct sha384_test const *test)
{
	struct sha512_context ctx;
	uint8_t digest[SHA384_DIGEST_SIZE];
	char digest_s[SHA384_DIGEST_SIZE * 2 + 1];

	sha384_init(&ctx);

	sha512_update(&ctx, test->input, strlen(test->input));
	sha384_digest(&ctx, digest);

	for (int i = 0; i < SHA384_DIGEST_SIZE; i += 1) {
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
		int result = test_sha384(&TESTS[i]);

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
