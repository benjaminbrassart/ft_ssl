/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:46:54 by bbrassar          #+#    #+#             */
/*   Updated: 2025/06/02 18:43:09 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "base64.h"

#include <stdint.h>
#include <stdlib.h>

static char const BASE64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			     "abcdefghijklmnopqrstuvwxyz"
			     "0123456789"
			     "+/";

static void encode_full_block(uint8_t const in[3], char out[4])
{
	// c0 = BITS(7..2)(b0)
	// c1 = BITS(1..0)(b0) + BITS(7..4)(b1)
	// c2 = BITS(3..0)(b1) + BITS(7..6)(b2)
	// c3 = BITS(5..0)(b3)

	out[0] = BASE64[in[0] >> 2];
	out[1] = BASE64[((in[0] & 0x03) << 4) | (in[1] >> 4)];
	out[2] = BASE64[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
	out[3] = BASE64[in[2] & 0x3f];
}

static int char_to_sextet(char c, uint8_t *sextet)
{
	if (c >= 'A' && c <= 'Z') {
		*sextet = c - 'A';
	} else if (c >= 'a' && c <= 'z') {
		*sextet = (c - 'a') + 26;
	} else if (c >= '0' && c <= '9') {
		*sextet = (c - '0') + 52;
	} else if (c == '+') {
		*sextet = 62;
	} else if (c == '/') {
		*sextet = 63;
	} else {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

static int decode_full_block(char const in[4], uint8_t out[3])
{
	uint8_t sextets[4];

	for (int i = 0; i < 4; i += 1) {
		int result;

		result = char_to_sextet(in[i], &sextets[i]);
		if (result != EXIT_SUCCESS) {
			return result;
		}
	}

	out[0] = (sextets[0] << 2) | (sextets[1] >> 4);
	out[1] = (sextets[1] << 4) | (sextets[2] >> 2);
	out[2] = (sextets[2] << 6) | sextets[3];

	return EXIT_SUCCESS;
}

void base64_enc_init(struct base64_encoder *enc, void *ctx,
		     base64_encode_yield *yield)
{
	enc->ctx = ctx;
	enc->yield = yield;
	enc->length = 0;
}

int base64_enc_update(struct base64_encoder *enc, void const *data, size_t len)
{
	uint8_t const *bytes = data;

	for (size_t i = 0; i < len; i += 1) {
		enc->buffer[enc->length] = bytes[i];
		enc->length += 1;
		if (enc->length % 3 == 0) {
			char out[4];

			encode_full_block(enc->buffer, out);
			enc->length = 0;

			int res = enc->yield(enc->ctx, out);

			if (res != EXIT_SUCCESS) {
				return res;
			}
		}
	}

	return EXIT_SUCCESS;
}

int base64_enc_finalize(struct base64_encoder *enc, unsigned pad)
{
	if (enc->length == 0) {
		return enc->yield(enc->ctx, "\0\0\0\0");
	}

	char pad_char = pad ? '=' : '\0';
	char out[4] = { pad_char, pad_char, pad_char, pad_char };
	uint8_t index[4] = { 0xff, 0xff, 0xff, 0xff };

	index[0] = (enc->buffer[0] >> 2);
	index[1] = ((enc->buffer[0] & 0x03) << 4);

	if (enc->length > 1) {
		index[1] |= (enc->buffer[1] >> 4);
		index[2] = ((enc->buffer[1] & 0x0f) << 2);
	}

	if (enc->length > 2) {
		index[2] |= (enc->buffer[2] >> 6);
		index[3] = (enc->buffer[2] & 0x3f);
	}

	for (int i = 0; i < 4; i += 1) {
		if (index[i] == 0xff) {
			break;
		}
		out[i] = BASE64[index[i]];
	}

	return enc->yield(enc->ctx, out);
}
