/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 15:51:47 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/29 09:54:22 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file md5.c
 * This is an implementation of the MD5 hashing algorithm.
 *
 * @see https://en.wikipedia.org/wiki/MD5#Pseudocode
 */

#include "ft_ssl/md5.h"
#include "ft_ssl/digest.h"
#include "ft_ssl/rotate.h"
#include "libft/ft.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>

/**
 * Loop through the internal buffer of a context and update the hash
 *
 * @param context the context to update
 */
static void __md5_step(Md5Context* context);

static uint32_t const K[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391,
};

static uint8_t const S[64] = {
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20, 5, 9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21,
};

void md5_init(Md5Context* context)
{
    context->length = 0;
    context->hash_vars[A] = 0x67452301;
    context->hash_vars[B] = 0xefcdab89;
    context->hash_vars[C] = 0x98badcfe;
    context->hash_vars[D] = 0x10325476;
}

void md5_update(Md5Context* context, void const* data, size_t len)
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
            __md5_step(context);
    }
}

void md5_digest(Md5Context* context, void* output)
{
    static uint8_t const _BIT = 0x80;
    static uint8_t const _PADDING[64] = {};
    uint32_t* bytes = (uint32_t*)output;

    uint64_t original_length = context->length * 8;

    md5_update(context, &_BIT, 1);

    size_t len_mod = context->length % 64;
    size_t padding_size;

    if (len_mod <= 56)
        padding_size = 56 - len_mod;
    else
        padding_size = (64 - len_mod) + 56;

    md5_update(context, _PADDING, padding_size);
    md5_update(context, &original_length, sizeof (original_length));

    bytes[A] = context->hash_vars[A];
    bytes[B] = context->hash_vars[B];
    bytes[C] = context->hash_vars[C];
    bytes[D] = context->hash_vars[D];
}

static void __md5_step(Md5Context* context)
{
    uint32_t vars[4];

    ft_memcpy(vars, context->hash_vars, sizeof (vars));

    for (uint32_t i = 0; i < 64; i += 1)
    {
        uint32_t f;
        uint32_t g;

        switch (i / 16)
        {
            case 0:
                f = (vars[B] & vars[C]) | (~vars[B] & vars[D]);
                g = i;
                break;
            case 1:
                f = (vars[D] & vars[B]) | (~vars[D] & vars[C]);
                g = (5 * i + 1) % 16;
                break;
            case 2:
                f = vars[B] ^ vars[C] ^ vars[D];
                g = (3 * i + 5) % 16;
                break;
            case 3:
                f = vars[C] ^ (vars[B] | ~vars[D]);
                g = (7 * i) % 16;
                break;
        }

        f = f + vars[A] + K[i] + context->buffer.u32[g];
        vars[A] = vars[D];
        vars[D] = vars[C];
        vars[C] = vars[B];
        vars[B] = vars[B] + rotate_left_u32(f, S[i]);
    }

    context->hash_vars[A] += vars[A];
    context->hash_vars[B] += vars[B];
    context->hash_vars[C] += vars[C];
    context->hash_vars[D] += vars[D];
}
