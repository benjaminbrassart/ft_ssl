/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 09:30:23 by bbrassar          #+#    #+#             */
/*   Updated: 2025/06/02 12:41:18 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash/md5.h"

struct hash_algorithm const ALGORITHM_MD5 = {
	.name_pretty = "MD5",
	.init = (hash_init_t *)md5_init,
	.update = (hash_update_t *)md5_update,
	.digest = (hash_digest_t *)md5_digest,
	.digest_size = MD5_DIGEST_SIZE,
	.context_size = sizeof(struct md5_context),
};

static inline uint32_t lrot32(uint32_t n, int r)
{
	return (n << r) | (n >> (32 - r));
}

/* clang-format off */

static uint32_t const K[64] = {
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
};

static int const S[64] = {
	7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
	5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
	4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
	6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,
};

/* clang-format on */

void md5_init(struct md5_context *ctx)
{
	ctx->a = 0x67452301;
	ctx->b = 0xefcdab89;
	ctx->c = 0x98badcfe;
	ctx->d = 0x10325476;
	ctx->length = 0;
}

static void md5_compress(struct md5_context *ctx)
{
	uint32_t a = ctx->a;
	uint32_t b = ctx->b;
	uint32_t c = ctx->c;
	uint32_t d = ctx->d;
	uint32_t message[16];

	for (int i = 0; i < 16; i += 1) {
		message[i] = (((uint32_t)ctx->buffer[i * 4 + 0]) << 0) |
			     (((uint32_t)ctx->buffer[i * 4 + 1]) << 8) |
			     (((uint32_t)ctx->buffer[i * 4 + 2]) << 16) |
			     (((uint32_t)ctx->buffer[i * 4 + 3]) << 24);
	}

	for (int i = 0; i < 64; i += 1) {
		uint32_t f;
		int g;

		if (i < 16) {
			f = (b & c) | (~b & d);
			g = i;
		} else if (i < 32) {
			f = (d & b) | (~d & c);
			g = (5 * i + 1) % 16;
		} else if (i < 48) {
			f = b ^ c ^ d;
			g = (3 * i + 5) % 16;
		} else {
			f = c ^ (b | ~d);
			g = (7 * i) % 16;
		}

		f = f + a + K[i] + message[g];
		a = d;
		d = c;
		c = b;
		b = b + lrot32(f, S[i]);
	}

	ctx->a += a;
	ctx->b += b;
	ctx->c += c;
	ctx->d += d;
}

static void md5_push_byte(struct md5_context *ctx, uint8_t byte)
{
	ctx->buffer[ctx->length % MD5_BLOCK_SIZE] = byte;
	ctx->length += 1;
	if (ctx->length % MD5_BLOCK_SIZE == 0) {
		md5_compress(ctx);
	}
}

void md5_update(struct md5_context *ctx, void const *data, size_t len)
{
	uint8_t const *buffer = data;

	for (size_t i = 0; i < len; i += 1) {
		md5_push_byte(ctx, buffer[i]);
	}
}

void md5_digest(struct md5_context *ctx, uint8_t digest[MD5_DIGEST_SIZE])
{
	uint8_t original_length[8];

	{
		uint64_t const bits = ctx->length * 8;

		for (int i = 0; i < 8; i += 1) {
			original_length[i] = (bits >> (i * 8)) & 0xff;
		}
	}

	md5_push_byte(ctx, 0x80); // append bit

	// pad with zeroes
	while (ctx->length % MD5_BLOCK_SIZE != 56) {
		md5_push_byte(ctx, 0x00);
	}

	md5_update(ctx, original_length, 8);

	uint32_t const registers[4] = { ctx->a, ctx->b, ctx->c, ctx->d };

	for (int i = 0; i < 4; i += 1) {
		digest[i * 4 + 0] = (registers[i] >> 0) & 0xff;
		digest[i * 4 + 1] = (registers[i] >> 8) & 0xff;
		digest[i * 4 + 2] = (registers[i] >> 16) & 0xff;
		digest[i * 4 + 3] = (registers[i] >> 24) & 0xff;
	}
}
