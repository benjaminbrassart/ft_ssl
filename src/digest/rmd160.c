/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rmd160.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 10:14:19 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/29 11:32:02 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl/rmd160.h"
#include "ft_ssl/digest.h"
#include "ft_ssl/rotate.h"
#include "libft/ft_byteswap.h"
#include <stddef.h>
#include <stdint.h>

/**
 * @file This is an implementation of the RIPEMD-160 hashing function.
 *
 * @see https://homes.esat.kuleuven.be/~bosselae/ripemd160/pdf/AB-9601/AB-9601.pdf
 */

static void __rmd160_step(Rmd160Context* context);

/**
 * Main selection of message word
 */
static uint8_t const R[80] = {
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8,
    3, 10, 14, 4, 9, 15, 8, 1, 2, 7, 0, 6, 13, 11, 5, 12,
    1, 9, 11, 10, 0, 8, 12, 4, 13, 3, 7, 15, 14, 5, 6, 2,
    4, 0, 5, 9, 7, 12, 2, 10, 14, 1, 3, 8, 11, 6, 15, 13,
};

/**
 * Parallel selection of message word
 */
static uint8_t const RP[80] = {
    5, 14, 7, 0, 9, 2, 11, 4, 13, 6, 15, 8, 1, 10, 3, 12,
    6, 11, 3, 7, 0, 13, 5, 10, 14, 15, 8, 12, 4, 9, 1, 2,
    15, 5, 1, 3, 7, 14, 6, 9, 11, 8, 12, 2, 10, 0, 4, 13,
    8, 6, 4, 1, 3, 11, 15, 0, 5, 12, 2, 13, 9, 7, 10, 14,
    12, 15, 10, 4, 1, 5, 8, 7, 6, 2, 13, 14, 0, 3, 9, 11,
};

/**
 * Main left rotation amounts
 */
static uint8_t const S[80] = {
    11, 14, 15, 12, 5, 8, 7, 9, 11, 13, 14, 15, 6, 7, 9, 8,
    7, 6, 8, 13, 11, 9, 7, 15, 7, 12, 15, 9, 11, 7, 13, 12,
    11, 13, 6, 7, 14, 9, 13, 15, 14, 8, 13, 6, 5, 12, 7, 5,
    11, 12, 14, 15, 14, 15, 9, 8, 9, 14, 5, 6, 8, 6, 5, 12,
    9, 15, 5, 11, 6, 8, 13, 12, 5, 12, 13, 14, 11, 8, 5, 6,
};

/**
 * Parallel left rotation amounts
 */
static uint8_t const SP[80] = {
    8, 9, 9, 11, 13, 15, 15, 5, 7, 7, 8, 11, 14, 14, 12, 6,
    9, 13, 15, 7, 12, 8, 9, 11, 7, 7, 12, 7, 6, 15, 13, 11,
    9, 7, 15, 11, 8, 6, 6, 14, 12, 13, 5, 14, 13, 13, 7, 5,
    15, 5, 8, 11, 14, 14, 6, 14, 6, 9, 12, 9, 12, 5, 15, 8,
    8, 5, 12, 9, 12, 5, 14, 6, 8, 13, 6, 5, 15, 13, 11, 11,
};

void rmd160_init(Rmd160Context* context)
{
    context->length = 0;
    context->hash_vars[A] = 0x67452301;
    context->hash_vars[B] = 0xefcdab89;
    context->hash_vars[C] = 0x98badcfe;
    context->hash_vars[D] = 0x10325476;
    context->hash_vars[E] = 0xc3d2e1f0;
}

void rmd160_update(Rmd160Context* context, void const* data, size_t len)
{
    uint8_t const* bytes = (uint8_t const*)data;
    size_t n = len;

    while (n > 0)
    {
        context->buffer.u8[context->length % 64] = *bytes;

        bytes += 1;
        n -= 1;

        if (context->length == UINT64_MAX)
            context->length = 0;
        else
            context->length += 1;

        if (context->length % 64 == 0)
            __rmd160_step(context);
    }
}

void rmd160_digest(Rmd160Context* context, void* output)
{
    static uint8_t const _BIT = 0x80;
    static uint8_t const _PADDING[64] = {};
    uint32_t* bytes = (uint32_t*)output;

    uint64_t original_length = context->length * 8;

    rmd160_update(context, &_BIT, 1);

    size_t len_mod = context->length % 64;
    size_t padding_size;

    if (len_mod <= 56)
        padding_size = 56 - len_mod;
    else
        padding_size = (64 - len_mod) + 56;

    rmd160_update(context, _PADDING, padding_size);
    rmd160_update(context, &original_length, sizeof (original_length));

    bytes[A] = context->hash_vars[A];
    bytes[B] = context->hash_vars[B];
    bytes[C] = context->hash_vars[C];
    bytes[D] = context->hash_vars[D];
    bytes[E] = context->hash_vars[E];
}

static void __rmd160_step(Rmd160Context* context)
{
    // normal hash variables
    uint32_t vars[5];

    // parallel hash variables
    uint32_t pvars[5];

    vars[A] = pvars[A] = context->hash_vars[A];
    vars[B] = pvars[B] = context->hash_vars[B];
    vars[C] = pvars[C] = context->hash_vars[C];
    vars[D] = pvars[D] = context->hash_vars[D];
    vars[E] = pvars[E] = context->hash_vars[E];

    for (int i = 0; i < 16; i += 1)
        context->buffer.u32[i] = ft_bswap_32(context->buffer.u32[i]);

    for (int i = 0; i < 80; i += 1)
    {
        uint32_t f;
        uint32_t k;
        uint32_t kp;

        switch (i / 5)
        {
            case 0:
                f = vars[B] ^ vars[C] ^ vars[D];
                k = 0x00000000;
                kp = 0x5c4dd124;
                break;
            case 1:
                f = (vars[B] & vars[C]) | (~vars[B] & vars[D]);
                k = 0x5a827999;
                kp = 0x6d703ef3;
                break;
            case 2:
                f = (vars[B] | ~vars[C]) ^ vars[D];
                k = 0x6ed9eba1;
                kp = 0x7a6d76e9;
                break;
            case 3:
                f = (vars[B] & vars[D]) | (vars[C] & ~vars[D]);
                k = 0x8f1bbcdc;
                kp = 0x00000000;
                break;
            case 4:
                f = vars[B] ^ (vars[C] | ~vars[D]);
                k = 0xa953fd4e;
                kp = 0x50a28be6;
                break;
        }

        // main
        {
            uint32_t temp = rotate_left_u32(vars[A] + f + context->buffer.u32[R[i]] + k, S[i]) + vars[E];

            vars[A] = vars[E];
            vars[E] = vars[D];
            vars[D] = rotate_left_u32(vars[C], 10);
            vars[C] = vars[B];
            vars[B] = temp;
        }

        // parallel
        {
            uint32_t temp = rotate_left_u32(pvars[A] + f + context->buffer.u32[RP[i]] + kp, SP[i]) + pvars[E];

            pvars[A] = pvars[E];
            pvars[E] = pvars[D];
            pvars[D] = rotate_left_u32(pvars[C], 10);
            pvars[C] = pvars[B];
            pvars[B] = temp;
        }
    }

    uint32_t temp = context->hash_vars[B] + vars[C] + pvars[D];

    context->hash_vars[B] = context->hash_vars[C] + vars[D] + pvars[E];
    context->hash_vars[C] = context->hash_vars[D] + vars[E] + pvars[A];
    context->hash_vars[D] = context->hash_vars[E] + vars[A] + pvars[B];
    context->hash_vars[E] = context->hash_vars[A] + vars[B] + pvars[C];
    context->hash_vars[A] = temp;
}
