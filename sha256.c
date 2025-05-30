/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:09:18 by bbrassar          #+#    #+#             */
/*   Updated: 2025/05/30 11:44:23 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sha2.h"

static inline uint32_t rrot32(uint32_t n, int r)
{
	return (n >> r) | (n << (32 - r));
}

/* clang-format off */

static uint32_t const K[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

/* clang-format on */

void sha256_init(struct sha256_context *ctx)
{
	ctx->a = 0x6a09e667;
	ctx->b = 0xbb67ae85;
	ctx->c = 0x3c6ef372;
	ctx->d = 0xa54ff53a;
	ctx->e = 0x510e527f;
	ctx->f = 0x9b05688c;
	ctx->g = 0x1f83d9ab;
	ctx->h = 0x5be0cd19;
	ctx->length = 0;
}

void sha224_init(struct sha256_context *ctx)
{
	ctx->a = 0xc1059ed8;
	ctx->b = 0x367cd507;
	ctx->c = 0x3070dd17;
	ctx->d = 0xf70e5939;
	ctx->e = 0xffc00b31;
	ctx->f = 0x68581511;
	ctx->g = 0x64f98fa7;
	ctx->h = 0xbefa4fa4;
	ctx->length = 0;
}

static void sha256_compress(struct sha256_context *ctx)
{
	uint32_t a = ctx->a;
	uint32_t b = ctx->b;
	uint32_t c = ctx->c;
	uint32_t d = ctx->d;
	uint32_t e = ctx->e;
	uint32_t f = ctx->f;
	uint32_t g = ctx->g;
	uint32_t h = ctx->h;
	uint32_t w[64];

	for (int i = 0; i < 16; i += 1) {
		w[i] = (((uint32_t)ctx->buffer[i * 4 + 0]) << 24) |
		       (((uint32_t)ctx->buffer[i * 4 + 1]) << 16) |
		       (((uint32_t)ctx->buffer[i * 4 + 2]) << 8) |
		       (((uint32_t)ctx->buffer[i * 4 + 3]) << 0);
	}

	for (int i = 16; i < 64; i += 1) {
		uint32_t const s0 = rrot32(w[i - 15], 7) ^
				    rrot32(w[i - 15], 18) ^ (w[i - 15] >> 3);
		uint32_t const s1 = rrot32(w[i - 2], 17) ^
				    rrot32(w[i - 2], 19) ^ (w[i - 2] >> 10);

		w[i] = w[i - 16] + s0 + w[i - 7] + s1;
	}

	for (int i = 0; i < 64; i += 1) {
		uint32_t const s1 = rrot32(e, 6) ^ rrot32(e, 11) ^
				    rrot32(e, 25);
		uint32_t const ch = (e & f) ^ (~e & g);
		uint32_t const temp1 = h + s1 + ch + K[i] + w[i];
		uint32_t const s0 = rrot32(a, 2) ^ rrot32(a, 13) ^
				    rrot32(a, 22);
		uint32_t const maj = (a & b) ^ (a & c) ^ (b & c);
		uint32_t const temp2 = s0 + maj;

		h = g;
		g = f;
		f = e;
		e = d + temp1;
		d = c;
		c = b;
		b = a;
		a = temp1 + temp2;
	}

	ctx->a += a;
	ctx->b += b;
	ctx->c += c;
	ctx->d += d;
	ctx->e += e;
	ctx->f += f;
	ctx->g += g;
	ctx->h += h;
}

static void sha256_push_byte(struct sha256_context *ctx, uint8_t byte)
{
	ctx->buffer[ctx->length % SHA256_BLOCK_SIZE] = byte;
	ctx->length += 1;
	if (ctx->length % SHA256_BLOCK_SIZE == 0) {
		sha256_compress(ctx);
	}
}

void sha256_update(struct sha256_context *ctx, void const *data, size_t len)
{
	uint8_t const *buffer = data;

	for (size_t i = 0; i < len; i += 1) {
		sha256_push_byte(ctx, buffer[i]);
	}
}

static void sha256_predigest(struct sha256_context *ctx)
{
	uint8_t original_length[8];

	{
		uint64_t const bits = ctx->length * 8;

		for (int i = 0; i < 8; i += 1) {
			original_length[i] = (bits >> ((7 - i) * 8)) & 0xff;
		}
	}

	sha256_push_byte(ctx, 0x80); // append bit

	// pad with zeroes
	while (ctx->length % SHA256_BLOCK_SIZE != 56) {
		sha256_push_byte(ctx, 0x00);
	}

	sha256_update(ctx, original_length, 8);
}

void sha256_digest(struct sha256_context *ctx,
		   uint8_t digest[SHA256_DIGEST_SIZE])
{
	sha256_predigest(ctx);

	uint32_t const registers[8] = { ctx->a, ctx->b, ctx->c, ctx->d,
					ctx->e, ctx->f, ctx->g, ctx->h };

	for (int i = 0; i < 8; i += 1) {
		digest[i * 4 + 0] = (registers[i] >> 24) & 0xff;
		digest[i * 4 + 1] = (registers[i] >> 16) & 0xff;
		digest[i * 4 + 2] = (registers[i] >> 8) & 0xff;
		digest[i * 4 + 3] = (registers[i] >> 0) & 0xff;
	}
}

void sha224_digest(struct sha256_context *ctx,
		   uint8_t digest[SHA224_DIGEST_SIZE])
{
	sha256_predigest(ctx);

	uint32_t const registers[7] = { ctx->a, ctx->b, ctx->c, ctx->d,
					ctx->e, ctx->f, ctx->g };

	for (int i = 0; i < 7; i += 1) {
		digest[i * 4 + 0] = (registers[i] >> 24) & 0xff;
		digest[i * 4 + 1] = (registers[i] >> 16) & 0xff;
		digest[i * 4 + 2] = (registers[i] >> 8) & 0xff;
		digest[i * 4 + 3] = (registers[i] >> 0) & 0xff;
	}
}
