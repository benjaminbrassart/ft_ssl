/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:05:58 by bbrassar          #+#    #+#             */
/*   Updated: 2025/05/30 11:07:26 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>
#include <stdint.h>

#define SHA256_DIGEST_SIZE 32
#define SHA256_DIGEST_SIZE 32

struct sha256_context {};

void sha256_init(struct sha256_context *ctx);

void sha256_update(struct sha256_context *ctx, void const *data, size_t len);

void sha256_digest(struct sha256_context *ctx,
		   uint8_t digest[SHA256_DIGEST_SIZE]);
