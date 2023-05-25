/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 18:09:33 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/25 18:12:22 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHA1_H
# define SHA1_H

# include "ft_ssl/cpp.h"
# include <stdint.h>
# include <stddef.h>

CPP_BEGIN

# define SHA1_OUT_SIZE (20)

typedef struct sha1_context
{
    uint32_t hash_vars[5];
    union
    {
        uint8_t u8[64];
        uint32_t u32[16];
    } buffer;
    uint64_t length;
} Sha1Context;

void sha1_init(Sha1Context* context);

void sha1_update(Sha1Context* context, void const* data, size_t len);

void sha1_digest(Sha1Context* context, void* output);

CPP_END

#endif // SHA1_H
