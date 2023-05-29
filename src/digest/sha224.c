/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha224.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 05:29:12 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/28 06:09:57 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl/sha2.h"
#include "ft_ssl/digest.h"
#include "libft/ft_byteswap.h"
#include <stddef.h>

void sha224_init(Sha256Context* context)
{
    context->length = 0;
    context->hash_vars[A] = 0xc1059ed8;
    context->hash_vars[B] = 0x367cd507;
    context->hash_vars[C] = 0x3070dd17;
    context->hash_vars[D] = 0xf70e5939;
    context->hash_vars[E] = 0xffc00b31;
    context->hash_vars[F] = 0x68581511;
    context->hash_vars[G] = 0x64f98fa7;
    context->hash_vars[H] = 0xbefa4fa4;
}

void sha224_update(Sha256Context* context, void const* data, size_t len)
{
    sha256_update(context, data, len);
}

void sha224_digest(Sha256Context* context, void* output)
{
    static uint8_t const _BIT = 0x80;
    static uint8_t const _PADDING[64] = {};
    uint32_t* hash = (uint32_t*)output;

    uint64_t original_length = ft_bswap_64(context->length * 8);

    sha224_update(context, &_BIT, 1);

    size_t len_mod = context->length % 64;
    size_t padding_size;

    if (len_mod <= 56)
        padding_size = 56 - len_mod;
    else
        padding_size = (64 - len_mod) + 56;

    sha224_update(context, _PADDING, padding_size);
    sha224_update(context, &original_length, sizeof (original_length));

    hash[A] = ft_bswap_32(context->hash_vars[A]);
    hash[B] = ft_bswap_32(context->hash_vars[B]);
    hash[C] = ft_bswap_32(context->hash_vars[C]);
    hash[D] = ft_bswap_32(context->hash_vars[D]);
    hash[E] = ft_bswap_32(context->hash_vars[E]);
    hash[F] = ft_bswap_32(context->hash_vars[F]);
    hash[G] = ft_bswap_32(context->hash_vars[G]);
}
