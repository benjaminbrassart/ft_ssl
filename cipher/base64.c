/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:46:54 by bbrassar          #+#    #+#             */
/*   Updated: 2025/06/02 13:38:50 by bbrassar         ###   ########.fr       */
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
