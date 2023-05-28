/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha384.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 05:47:57 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/28 07:00:11 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl/sha2.h"
#include "ft_ssl/digest.h"
#include "ft_ssl/byteswap.h"
#include "libft/ft_byteswap.h"
#include <stddef.h>

void sha384_init(Sha512Context* context)
{
    context->length = 0;
    context->hash_vars[A] = 0xcbbb9d5dc1059ed8;
    context->hash_vars[B] = 0x629a292a367cd507;
    context->hash_vars[C] = 0x9159015a3070dd17;
    context->hash_vars[D] = 0x152fecd8f70e5939;
    context->hash_vars[E] = 0x67332667ffc00b31;
    context->hash_vars[F] = 0x8eb44a8768581511;
    context->hash_vars[G] = 0xdb0c2e0d64f98fa7;
    context->hash_vars[H] = 0x47b5481dbefa4fa4;
}

void sha384_update(Sha512Context* context, void const* data, size_t len)
{
    sha512_update(context, data, len);
}

void sha384_digest(Sha512Context* context, void* output)
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
}
