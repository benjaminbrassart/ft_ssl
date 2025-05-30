/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha2.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:05:58 by bbrassar          #+#    #+#             */
/*   Updated: 2025/05/30 18:43:26 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "hash.h"

#include <stddef.h>
#include <stdint.h>

#define SHA224_DIGEST_SIZE 28

#define SHA256_BLOCK_SIZE 64
#define SHA256_DIGEST_SIZE 32

extern struct hash_algorithm const ALGORITHM_SHA224;
extern struct hash_algorithm const ALGORITHM_SHA256;
extern struct hash_algorithm const ALGORITHM_SHA384;
extern struct hash_algorithm const ALGORITHM_SHA512;

struct sha256_context {
	uint32_t a, b, c, d, e, f, g, h;
	uint8_t buffer[SHA256_BLOCK_SIZE];
	uint64_t length;
};

void sha224_init(struct sha256_context *ctx);

void sha224_digest(struct sha256_context *ctx,
		   uint8_t digest[SHA224_DIGEST_SIZE]);

void sha256_init(struct sha256_context *ctx);

void sha256_update(struct sha256_context *ctx, void const *data, size_t len);

void sha256_digest(struct sha256_context *ctx,
		   uint8_t digest[SHA256_DIGEST_SIZE]);

#define SHA384_DIGEST_SIZE 48

#define SHA512_BLOCK_SIZE 128
#define SHA512_DIGEST_SIZE 64

struct sha512_context {
	uint64_t a, b, c, d, e, f, g, h;
	uint8_t buffer[SHA512_BLOCK_SIZE];
	uint64_t length[2];
};

void sha384_init(struct sha512_context *ctx);

void sha384_digest(struct sha512_context *ctx,
		   uint8_t digest[SHA384_DIGEST_SIZE]);

void sha512_init(struct sha512_context *ctx);

void sha512_update(struct sha512_context *ctx, void const *data, size_t len);

void sha512_digest(struct sha512_context *ctx,
		   uint8_t digest[SHA512_DIGEST_SIZE]);
