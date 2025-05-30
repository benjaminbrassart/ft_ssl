/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 09:30:23 by bbrassar          #+#    #+#             */
/*   Updated: 2025/05/30 09:40:34 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "md5.h"

void md5_init(struct md5_context *ctx)
{
	ctx->a = 0x67452301;
	ctx->b = 0xefcdab89;
	ctx->c = 0x98badcfe;
	ctx->d = 0x10325476;
	ctx->length = 0;
}

static void md5_step(struct md5_context *ctx)
{
	// TODO implement internal logic
}

static void md5_push_byte(struct md5_context *ctx, uint8_t byte)
{
	ctx->buffer[ctx->length % MD5_BLOCK_SIZE] = byte;
	ctx->length += 1;
	if (ctx->length % MD5_BLOCK_SIZE == 0) {
		md5_step(ctx);
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
	uint32_t *output = (uint32_t *)digest;
	uint64_t const original_length_bits = ctx->length * 8;

	md5_push_byte(ctx, 0x80); // append bit

	// pad with zeroes
	while (ctx->length % MD5_BLOCK_SIZE != 56) {
		md5_push_byte(ctx, 0x00);
	}

	md5_update(ctx, &original_length_bits, sizeof(original_length_bits));
	output[0] = ctx->a;
	output[1] = ctx->b;
	output[2] = ctx->c;
	output[3] = ctx->d;
}
