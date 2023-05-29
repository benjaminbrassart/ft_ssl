/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rmd160.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/29 10:19:43 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/29 11:26:06 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RMD160_H
# define RMD160_H

# include "ft_ssl/cpp.h"
# include <stddef.h>
# include <stdint.h>

CPP_BEGIN

# define RMD160_OUT_SIZE (20)

typedef struct rmd160_context
{
    uint32_t hash_vars[5];
    uint64_t length;
    union
    {
        uint8_t u8[64];
        // uint16_t u16[32];
        uint32_t u32[16];
    } buffer;
} Rmd160Context;

void rmd160_init(Rmd160Context* context);

void rmd160_update(Rmd160Context* context, void const* data, size_t len);

void rmd160_digest(Rmd160Context* context, void* output);

CPP_END

#endif // RMD160_H
