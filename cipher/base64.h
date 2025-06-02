/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:46:41 by bbrassar          #+#    #+#             */
/*   Updated: 2025/06/02 18:42:59 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>
#include <stdint.h>

typedef int base64_encode_yield(void *ctx, char const data[4]);

struct base64_encoder {
	void *ctx;
	base64_encode_yield *yield;
	uint8_t buffer[3];
	size_t length;
};

void base64_enc_init(struct base64_encoder *enc, void *ctx,
		     base64_encode_yield *yield);

int base64_enc_update(struct base64_encoder *enc, void const *data, size_t len);

int base64_enc_finalize(struct base64_encoder *enc, unsigned pad);
