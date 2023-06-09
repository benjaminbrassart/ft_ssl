/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha512.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 06:16:27 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/29 09:54:54 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl/sha2.h"
#include "ft_ssl/digest.h"
#include "ft_ssl/rotate.h"
#include "ft_ssl/byteswap.h"
#include "libft/ft_byteswap.h"
#include <stddef.h>
#include <stdint.h>

static void __sha512_step(Sha512Context* context);

static uint64_t const K[80] = {
    0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc, 0x3956c25bf348b538,
    0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118, 0xd807aa98a3030242, 0x12835b0145706fbe,
    0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2, 0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235,
    0xc19bf174cf692694, 0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
    0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5, 0x983e5152ee66dfab,
    0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4, 0xc6e00bf33da88fc2, 0xd5a79147930aa725,
    0x06ca6351e003826f, 0x142929670a0e6e70, 0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed,
    0x53380d139d95b3df, 0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
    0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30, 0xd192e819d6ef5218,
    0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8, 0x19a4c116b8d2d0c8, 0x1e376c085141ab53,
    0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8, 0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373,
    0x682e6ff3d6b2b8a3, 0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
    0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b, 0xca273eceea26619c,
    0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178, 0x06f067aa72176fba, 0x0a637dc5a2c898a6,
    0x113f9804bef90dae, 0x1b710b35131c471b, 0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc,
    0x431d67c49c100d4c, 0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817,
};

void sha512_init(Sha512Context* context)
{
    context->length = 0;
    context->hash_vars[A] = 0x6a09e667f3bcc908;
    context->hash_vars[B] = 0xbb67ae8584caa73b;
    context->hash_vars[C] = 0x3c6ef372fe94f82b;
    context->hash_vars[D] = 0xa54ff53a5f1d36f1;
    context->hash_vars[E] = 0x510e527fade682d1;
    context->hash_vars[F] = 0x9b05688c2b3e6c1f;
    context->hash_vars[G] = 0x1f83d9abfb41bd6b;
    context->hash_vars[H] = 0x5be0cd19137e2179;
}

void sha512_update(Sha512Context* context, void const* data, size_t len)
{
    uint8_t const* bytes = (uint8_t const*)data;
    size_t n = len;

    while (n > 0)
    {
        context->buffer.u8[context->length % 128] = *bytes;

        bytes += 1;
        n -= 1;

        if (context->length == UINT64_MAX)
            context->length = 0;
        else
            context->length += 1;

        if (context->length % 128 == 0)
            __sha512_step(context);
    }
}

void sha512_digest(Sha512Context* context, void* output)
{
    static uint8_t const _BIT = 0x80;
    static uint8_t const _PADDING[128] = {};
    uint64_t* hash = (uint64_t*)output;

    __uint128_t original_length = ft_bswap_128(context->length * 8);

    sha512_update(context, &_BIT, 1);

    size_t len_mod = context->length % 128;
    size_t padding_size;

    if (len_mod <= 112)
        padding_size = 112 - len_mod;
    else
        padding_size = (128 - len_mod) + 112;

    sha512_update(context, _PADDING, padding_size);
    sha512_update(context, &original_length, sizeof (original_length));

    hash[A] = ft_bswap_64(context->hash_vars[A]);
    hash[B] = ft_bswap_64(context->hash_vars[B]);
    hash[C] = ft_bswap_64(context->hash_vars[C]);
    hash[D] = ft_bswap_64(context->hash_vars[D]);
    hash[E] = ft_bswap_64(context->hash_vars[E]);
    hash[F] = ft_bswap_64(context->hash_vars[F]);
    hash[G] = ft_bswap_64(context->hash_vars[G]);
    hash[H] = ft_bswap_64(context->hash_vars[H]);
}

static void __sha512_step(Sha512Context* context)
{
    uint64_t w[80];

    for (int i = 0; i < 16; i += 1)
        w[i] = ft_bswap_64(context->buffer.u64[i]);

    {
        uint64_t sigma0;
        uint64_t sigma1;

        for (int i = 16; i < 80; i += 1)
        {
            sigma0 = (rotate_right_u64(w[i - 15], 1) ^ rotate_right_u64(w[i - 15], 8)) ^ (w[i - 15] >> 7);
            sigma1 = (rotate_right_u64(w[i - 2], 19) ^ rotate_right_u64(w[i - 2], 61)) ^ (w[i - 2] >> 6);
            w[i] = w[i - 16] + sigma0 + w[i - 7] + sigma1;
        }
    }

    uint64_t vars[8];

    for (int i = 0; i < 8; i += 1)
        vars[i] = context->hash_vars[i];

    {
        uint64_t sigma0;
        uint64_t sigma1;

        for (int i = 0; i < 80; i += 1)
        {
            uint64_t ch;
            uint64_t maj;
            uint64_t temp1;
            uint64_t temp2;

            sigma1 = rotate_right_u64(vars[E], 14) ^ rotate_right_u64(vars[E], 18) ^ rotate_right_u64(vars[E], 41);
            ch = (vars[E] & vars[F]) ^ (~vars[E] & vars[G]);
            temp1 = vars[H] + sigma1 + ch + K[i] + w[i];

            sigma0 = rotate_right_u64(vars[A], 28) ^ rotate_right_u64(vars[A], 34) ^ rotate_right_u64(vars[A], 39);
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
