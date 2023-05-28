/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha2.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 23:38:15 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/28 06:15:25 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHA2_H
# define SHA2_H

# include "ft_ssl/cpp.h"
# include <stddef.h>
# include <stdint.h>

CPP_BEGIN

# define SHA224_OUT_SIZE (28)
# define SHA256_OUT_SIZE (32)
# define SHA384_OUT_SIZE (48)
# define SHA512_OUT_SIZE (64)

typedef struct sha256_context
{
    uint64_t length;
    uint32_t hash_vars[8];
    union
    {
        uint8_t u8[64];
        uint32_t u32[16];
    } buffer;
} Sha256Context;

typedef struct sha512_context
{
    uint64_t length;
    uint64_t hash_vars[8];
    union
    {
        uint8_t u8[128];
        uint64_t u64[16];
    };
} Sha512Context;

void sha224_init(Sha256Context* context);
void sha256_init(Sha256Context* context);
void sha384_init(Sha512Context* context);
void sha512_init(Sha512Context* context);

void sha224_update(Sha256Context* context, void const* data, size_t len);
void sha256_update(Sha256Context* context, void const* data, size_t len);
void sha384_update(Sha512Context* context, void const* data, size_t len);
void sha512_update(Sha512Context* context, void const* data, size_t len);

void sha224_digest(Sha256Context* context, void* output);
void sha256_digest(Sha256Context* context, void* output);
void sha384_digest(Sha512Context* context, void* output);
void sha512_digest(Sha512Context* context, void* output);

CPP_END

#endif // SHA2_H
