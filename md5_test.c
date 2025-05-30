/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_test.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 09:47:31 by bbrassar          #+#    #+#             */
/*   Updated: 2025/05/30 10:59:05 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "md5.c"
#include "md5.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 64
#define sizeof_array(Arr) (sizeof(Arr) / sizeof((Arr)[0]))

struct md5_test {
	char const *input;
	char const digest[MD5_DIGEST_SIZE * 2 + 1];
};

static struct md5_test const TESTS[] = {
	{
		.input = "",
		.digest = "d41d8cd98f00b204e9800998ecf8427e",
	},
	{
		.input = "The quick brown fox jumps over the lazy dog",
		.digest = "9e107d9d372bb6826bd81d3542a419d6",
	},
	{
		.input = "The quick brown fox jumps over the lazy dog.",
		.digest = "e4d909c290d0fb1ca068ffaddf22cbd0",
	},
};

static int test_md5(struct md5_test const *test)
{
	struct md5_context ctx;
	uint8_t digest[MD5_DIGEST_SIZE];
	char digest_s[MD5_DIGEST_SIZE * 2 + 1];

	md5_init(&ctx);

	md5_update(&ctx, test->input, strlen(test->input));
	md5_digest(&ctx, digest);

	for (int i = 0; i < MD5_DIGEST_SIZE; i += 1) {
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
	// do {
	// 	size_t rc = fread(buffer, 1, BUFFER_SIZE, stdin);

	// 	md5_update(&ctx, buffer, rc);
	// } while (!ferror(f) && !feof(f));

	// if (ferror(f)) {
	// 	fprintf(stderr, "error: %m\n");
	// 	return EXIT_FAILURE;
	// }

	int ok = 0;
	int ko = 0;

	for (size_t i = 0; i < sizeof_array(TESTS); i += 1) {
		int result = test_md5(&TESTS[i]);

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
