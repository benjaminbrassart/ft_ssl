/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 09:30:36 by bbrassar          #+#    #+#             */
/*   Updated: 2025/05/30 09:38:00 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>
#include <stdint.h>

#define MD5_BLOCK_SIZE 64
#define MD5_DIGEST_SIZE 16

struct md5_context {
	uint32_t a, b, c, d;
	uint8_t buffer[MD5_BLOCK_SIZE];
	uint64_t length;
};

void md5_init(struct md5_context *ctx);

void md5_update(struct md5_context *ctx, void const *data, size_t len);

void md5_digest(struct md5_context *ctx, uint8_t digest[MD5_DIGEST_SIZE]);
