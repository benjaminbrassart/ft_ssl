/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:05:58 by bbrassar          #+#    #+#             */
/*   Updated: 2025/05/30 11:14:30 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>
#include <stdint.h>

#define SHA256_BLOCK_SIZE 64
#define SHA256_DIGEST_SIZE 32

struct sha256_context {
	uint32_t a, b, c, d, e, f, g, h;
	uint8_t buffer[SHA256_BLOCK_SIZE];
	uint64_t length;
};

void sha256_init(struct sha256_context *ctx);

void sha256_update(struct sha256_context *ctx, void const *data, size_t len);

void sha256_digest(struct sha256_context *ctx,
		   uint8_t digest[SHA256_DIGEST_SIZE]);
