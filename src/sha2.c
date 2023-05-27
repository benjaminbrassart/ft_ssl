/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 00:20:51 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/27 19:56:27 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl/sha2.h"
#include "ft_ssl/rotate.h"
#include "libft/ft.h"
#include "libft/ft_byteswap.h"
#include <byteswap.h>

#define A 0
#define B 1
#define C 2
#define D 3
#define E 4
#define F 5
#define G 6
#define H 7

static uint32_t const HASH_VARS_256[8];

static void __sha224_init(Sha2Context* context);
static void __sha256_init(Sha2Context* context);
static void __sha384_init(Sha2Context* context);
static void __sha512_init(Sha2Context* context);

static void __sha224_update(Sha2Context* context, void const* data, size_t len);
static void __sha256_update(Sha2Context* context, void const* data, size_t len);
static void __sha384_update(Sha2Context* context, void const* data, size_t len);
static void __sha512_update(Sha2Context* context, void const* data, size_t len);

static void __sha224_digest(Sha2Context* context, void* output);
static void __sha256_digest(Sha2Context* context, void* output);
static void __sha384_digest(Sha2Context* context, void* output);
static void __sha512_digest(Sha2Context* context, void* output);

static void __sha224_step(Sha2Context* context) __attribute__((unused));
static void __sha256_step(Sha2Context* context);
static void __sha384_step(Sha2Context* context) __attribute__((unused));
static void __sha512_step(Sha2Context* context) __attribute__((unused));

static uint32_t const K_32[64] = {
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

static uint32_t const HASH_VARS_256[8] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19,
};

void sha2_init(Sha2Context* context, Sha2Algorithm algorithm)
{
    void (*init_fn)(Sha2Context*);

    context->alg = algorithm;
    context->length = 0;

    switch (algorithm)
    {
        case SHA224: init_fn = __sha224_init; break;
        case SHA256: init_fn = __sha256_init; break;
        case SHA384: init_fn = __sha384_init; break;
        case SHA512: init_fn = __sha512_init; break;
        default:     init_fn = NULL; break;
    }

    init_fn(context);
}

void sha2_update(Sha2Context* context, void const* data, size_t len)
{
    context->__update(context, data, len);
}

void sha2_digest(Sha2Context* context, void* output)
{
    context->__digest(context, output);
}

static void __sha224_init(Sha2Context* context)
{
    context->__update = __sha224_update;
    context->__digest = __sha224_digest;
}

static void __sha256_init(Sha2Context* context)
{
    context->__update = __sha256_update;
    context->__digest = __sha256_digest;

    for (int i = 0; i < 8; i += 1)
        context->data.sha256.hash_vars[i] = HASH_VARS_256[i];
}

static void __sha384_init(Sha2Context* context)
{
    context->__update = __sha384_update;
    context->__digest = __sha384_digest;
}

static void __sha512_init(Sha2Context* context)
{
    context->__update = __sha512_update;
    context->__digest = __sha512_digest;
}

static void __sha256_update(Sha2Context* context, void const* data, size_t len)
{
    uint8_t const* bytes = (uint8_t const*)data;
    size_t n = len;

    while (n > 0)
    {
        context->data.sha256.buffer.u8[context->length % 64] = *bytes;

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

static void __sha256_digest(Sha2Context* context, void* output)
{
    static uint8_t const _BIT = 0x80;
    static uint8_t const _PADDING[64] = {};
    uint32_t* hash = (uint32_t*)output;

    // TODO check for overflow;
    uint64_t original_length = context->length * 8;

    original_length = ft_bswap_64(original_length);

    context->__update(context, &_BIT, 1);

    size_t len_mod = context->length % 64;
    size_t padding_size;

    if (len_mod <= 56)
        padding_size = 56 - len_mod;
    else
        padding_size = (64 - len_mod) + 56;

    context->__update(context, _PADDING, padding_size);
    context->__update(context, &original_length, sizeof (original_length));

    for (int i = 0; i < 8; i += 1)
        hash[i] = bswap_32(context->data.sha256.hash_vars[i]);
}

static void __sha256_step(Sha2Context* context)
{
    uint32_t w[64];

    for (int i = 0; i < 16; i += 1)
        w[i] = ft_bswap_32(context->data.sha256.buffer.u32[i]);

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
        vars[i] = context->data.sha256.hash_vars[i];

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
            temp1 = vars[H] + sigma1 + ch + K_32[i] + w[i];

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

        for (int i = 0; i < 8; i += 1)
            context->data.sha256.hash_vars[i] += vars[i];
    }
}

static void __sha224_update(Sha2Context* context, void const* data, size_t len)
{
    (void)context;
    (void)data;
    (void)len;
}

static void __sha384_update(Sha2Context* context, void const* data, size_t len)
{
    (void)context;
    (void)data;
    (void)len;
}

static void __sha512_update(Sha2Context* context, void const* data, size_t len)
{
    (void)context;
    (void)data;
    (void)len;
}

static void __sha224_digest(Sha2Context* context, void* output)
{
    (void)context;
    (void)output;
}

static void __sha384_digest(Sha2Context* context, void* output)
{
    (void)context;
    (void)output;
}

static void __sha512_digest(Sha2Context* context, void* output)
{
    (void)context;
    (void)output;
}

static void __sha224_step(Sha2Context* context)
{
    (void)context;
}

static void __sha384_step(Sha2Context* context)
{
    (void)context;
}

static void __sha512_step(Sha2Context* context)
{
    (void)context;
}
