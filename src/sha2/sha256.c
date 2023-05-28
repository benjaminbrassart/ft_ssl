/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 05:32:43 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/28 06:12:49 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl/sha2.h"
#include "ft_ssl/digest.h"
#include "ft_ssl/rotate.h"
#include <libft/ft_byteswap.h>

static uint32_t const K[64];

static void __sha256_step(Sha256Context* context);

void sha256_init(Sha256Context* context)
{
    context->length = 0;
    context->hash_vars[A] = 0x6a09e667;
    context->hash_vars[B] = 0xbb67ae85;
    context->hash_vars[C] = 0x3c6ef372;
    context->hash_vars[D] = 0xa54ff53a;
    context->hash_vars[E] = 0x510e527f;
    context->hash_vars[F] = 0x9b05688c;
    context->hash_vars[G] = 0x1f83d9ab;
    context->hash_vars[H] = 0x5be0cd19;
}

void sha256_update(Sha256Context* context, void const* data, size_t len)
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
            __sha256_step(context);
    }
}

void sha256_digest(Sha256Context* context, void* output)
{
    static uint8_t const _BIT = 0x80;
    static uint8_t const _PADDING[64] = {};
    uint32_t* hash = (uint32_t*)output;

    uint64_t original_length = ft_bswap_64(context->length * 8);

    sha256_update(context, &_BIT, 1);

    size_t len_mod = context->length % 64;
    size_t padding_size;

    if (len_mod <= 56)
        padding_size = 56 - len_mod;
    else
        padding_size = (64 - len_mod) + 56;

    sha256_update(context, _PADDING, padding_size);
    sha256_update(context, &original_length, sizeof (original_length));

    hash[A] = ft_bswap_32(context->hash_vars[A]);
    hash[B] = ft_bswap_32(context->hash_vars[B]);
    hash[C] = ft_bswap_32(context->hash_vars[C]);
    hash[D] = ft_bswap_32(context->hash_vars[D]);
    hash[E] = ft_bswap_32(context->hash_vars[E]);
    hash[F] = ft_bswap_32(context->hash_vars[F]);
    hash[G] = ft_bswap_32(context->hash_vars[G]);
    hash[H] = ft_bswap_32(context->hash_vars[H]);
}

static uint32_t const K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
};

static void __sha256_step(Sha256Context* context)
{
    uint32_t w[64];

    for (int i = 0; i < 16; i += 1)
        w[i] = ft_bswap_32(context->buffer.u32[i]);

    {
        uint32_t sigma0;
        uint32_t sigma1;

        for (int i = 16; i < 64; i += 1)
        {
            sigma0 = (rotate_right_u32(w[i - 15], 7) ^ rotate_right_u32(w[i - 15], 18)) ^ (w[i - 15] >> 3);
            sigma1 = (rotate_right_u32(w[i - 2], 17) ^ rotate_right_u32(w[i - 2], 19)) ^ (w[i - 2] >> 10);
            w[i] = w[i - 16] + sigma0 + w[i - 7] + sigma1;
        }
    }

    uint32_t vars[8];

    for (int i = 0; i < 8; i += 1)
        vars[i] = context->hash_vars[i];

    {
        uint32_t sigma0;
        uint32_t sigma1;

        for (int i = 0; i < 64; i += 1)
        {
            uint32_t ch;
            uint32_t maj;
            uint32_t temp1;
            uint32_t temp2;

            sigma1 = rotate_right_u32(vars[E], 6) ^ rotate_right_u32(vars[E], 11) ^ rotate_right_u32(vars[E], 25);
            ch = (vars[E] & vars[F]) ^ (~vars[E] & vars[G]);
            temp1 = vars[H] + sigma1 + ch + K[i] + w[i];

            sigma0 = rotate_right_u32(vars[A], 2) ^ rotate_right_u32(vars[A], 13) ^ rotate_right_u32(vars[A], 22);
            maj = (vars[A] & vars[B]) ^ (vars[A] & vars[C]) ^ (vars[B] & vars[C]);
            temp2 = sigma0 + maj;

            vars[H] = vars[G];
            vars[G] = vars[F];
            vars[F] = vars[E];
            vars[E] = vars[D] + temp1;
            vars[D] = vars[C];
            vars[C] = vars[B];
            vars[B] = vars[A];
            vars[A] = temp1 + temp2;
        }

        context->hash_vars[A] += vars[A];
        context->hash_vars[B] += vars[B];
        context->hash_vars[C] += vars[C];
        context->hash_vars[D] += vars[D];
        context->hash_vars[E] += vars[E];
        context->hash_vars[F] += vars[F];
        context->hash_vars[G] += vars[G];
        context->hash_vars[H] += vars[H];
    }
}
