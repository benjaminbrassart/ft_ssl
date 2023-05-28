/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 18:13:55 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/28 02:14:32 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file sha1.c
 * This is an implementation of the SHA-1 algorithm.
 *
 * @see https://en.wikipedia.org/wiki/SHA-1#SHA-1_pseudocode
 */

#include "ft_ssl/sha1.h"
#include "ft_ssl/rotate.h"
#include "libft/ft.h"
#include <stddef.h>
#include <stdint.h>

#define A 0
#define B 1
#define C 2
#define D 3
#define E 4

static uint32_t const HASH_VARS[5];

static void __sha1_step(Sha1Context* context);

static uint32_t const HASH_VARS[5] = {
    0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0,
};

void sha1_init(Sha1Context* context)
{
    context->length = 0;
    for (int i = 0; i < 5; ++i)
        context->hash_vars[i] = HASH_VARS[i];
}

void sha1_update(Sha1Context* context, void const* data, size_t len)
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
            __sha1_step(context);
    }
}

void sha1_digest(Sha1Context* context, void* output)
{
    static uint8_t const _BIT = 0x80;
    static uint8_t const _PADDING[64] = {};
    uint32_t* hash = (uint32_t*)output;

    uint64_t original_length = context->length * 8;

    sha1_update(context, &_BIT, 1);

    size_t len_mod = context->length % 64;
    size_t padding_size;

    if (len_mod <= 56)
        padding_size = 56 - len_mod;
    else
        padding_size = (64 - len_mod) + 56;

    sha1_update(context, _PADDING, padding_size);
    sha1_update(context, &original_length, sizeof (original_length));

    for (int i = 0; i < 5; i += 1)
        hash[i] = context->hash_vars[i];
}

static void __sha1_step(Sha1Context* context)
{
    uint32_t vars[5];
    uint32_t w[80];

    for (int i = 0; i < 16; i += 1)
        w[i] = context->buffer.u32[i];

    for (int i = 16; i < 80; i += 1)
        w[i] = rotate_left_u32(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);

    for (int i = 0; i < 5; i += 1)
        vars[i] = context->hash_vars[i];

    for (int i = 0; i < 80; i += 1)
    {
        uint32_t f;
        uint32_t k;

        switch (i / 4)
        {
            case 0: //  0 - 19
                f = (vars[B] & vars[C]) | (~vars[B] & vars[D]);
                k = 0x5a827999;
                break;
            case 1: // 20 - 39
                f = vars[B] ^ vars[C] ^ vars[D];
                k = 0x6ed9eba1;
                break;
            case 2: // 40 - 59
                f = (vars[B] & vars[C]) | (vars[B] & vars[D]) | (vars[C] & vars[D]);
                k = 0x8f1bbcdc;
                break;
            case 3: // 60 - 79
                f = vars[B] ^ vars[C] ^ vars[D];
                k = 0xca62c1d6;
                break;
        }

        uint32_t temp = rotate_left_u32(vars[A], 5) +f + vars[E] + k + w[i];

        vars[E] = vars[D];
        vars[D] = vars[C];
        vars[C] = rotate_left_u32(vars[B], 30);
        vars[B] = vars[A];
        vars[A] = temp;
    }

    for (int i = 0; i < 5; i += 1)
        context->hash_vars[i] += vars[i];
}
