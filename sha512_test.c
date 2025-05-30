/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha512_test.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:50:26 by bbrassar          #+#    #+#             */
/*   Updated: 2025/05/30 11:51:13 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sha2.h"
#include "sha512.c"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 64
#define sizeof_array(Arr) (sizeof(Arr) / sizeof((Arr)[0]))

struct sha512_test {
	char const *input;
	char const digest[SHA512_DIGEST_SIZE * 2 + 1];
};

static struct sha512_test const TESTS[] = {
	{
		.input = "",
		.digest =
			"cf83e1357eefb8bdf1542850d66d8007d620e4050b5715dc83f4a921d36ce9ce47d0d13c5d85f2b0ff8318d2877eec2f63b931bd47417a81a538327af927da3e",
	},
	{
		.input = "1",
		.digest =
			"4dff4ea340f0a823f15d3f4f01ab62eae0e5da579ccb851f8db9dfe84c58b2b37b89903a740e1ee172da793a6e79d560e5f7f9bd058a12a280433ed6fa46510a",
	},
	{
		.input = "0000000000000000", // 16
		.digest =
			"56d01c4d1a698e26ac99eefdd77b9e98f1b909b407282830e8dffc18fb99f2159a44a1059f08c53e9bba17bc7695f35c720a207643dc8a11f7f93e470936b0f3",
	},
	{
		.input = "00000000000000000000000000000000", // 32
		.digest =
			"98b6d128682e280b74b324ca82a6bae6e8a3f7174e0605bfd52eb9948fad8984854ec08f7652f32055c4a9f12b69add4850481d9503a7f2225501671d6124648",
	},
	{
		.input =
			"000000000000000000000000000000000000000000000000", // 48
		.digest =
			"816b8e1d3305328634a836fe91e6ac125b4a641ef3d1c000ce513a27982fa9b8fc548f79da25adf5ab557c9041b410315c6515d2a9be2fe1720635fdf52c8968",
	},
	{
		.input =
			"0000000000000000000000000000000000000000000000000000000000000000", // 64
		.digest =
			"8f6beb3c0792f50c176800332f4468f76b4457b41d2f68e294cb46e53addbf5769a59eddf33e19394e8ab78e374b1bd33a680d26464fcd1174da226af9c8cd6e",
	},
	{
		.input = "The quick brown fox jumps over the lazy dog",
		.digest =
			"07e547d9586f6a73f73fbac0435ed76951218fb7d0c8d788a309d785436bbb642e93a252a954f23912547d1e8a3b5ed6e1bfd7097821233fa0538f3db854fee6",
	},
	{
		.input = "The quick brown fox jumps over the lazy dog.",
		.digest =
			"91ea1245f20d46ae9a037a989f54f1f790f0a47607eeb8a14d12890cea77a1bbc6c7ed9cf205e67b7f2b8fd4c7dfd3a7a8617e45f3c463d481c7e586c39ac1ed",
	},
};

static int test_sha512(struct sha512_test const *test)
{
	struct sha512_context ctx;
	uint8_t digest[SHA512_DIGEST_SIZE];
	char digest_s[SHA512_DIGEST_SIZE * 2 + 1];

	sha512_init(&ctx);

	sha512_update(&ctx, test->input, strlen(test->input));
	sha512_digest(&ctx, digest);

	for (int i = 0; i < SHA512_DIGEST_SIZE; i += 1) {
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
		int result = test_sha512(&TESTS[i]);

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
