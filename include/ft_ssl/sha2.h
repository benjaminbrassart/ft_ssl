/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha2.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 17:26:33 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/24 03:33:40 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file sha2.h
 */

#ifndef SHA2_H
# define SHA2_H

# include "ft_ssl/cpp.h"
# include <stddef.h>
# include <stdint.h>

CPP_BEGIN

# define SHA224_OUT_SIZE (224)
# define SHA256_OUT_SIZE (256)
# define SHA384_OUT_SIZE (384)
# define SHA512_OUT_SIZE (512)

typedef enum sha2_algorithm
{
    SHA224,
    SHA256,
    SHA384,
    SHA512,
} Sha2Algorithm;

typedef struct sha2_context Sha2Context;

struct sha2_context
{
    Sha2Algorithm alg;
    union
    {
        uint32_t u32[8];
        uint64_t u64[8];
    } hash_vars;
    uint8_t buffer[64];
    uint64_t length;
    void (*__update)(Sha2Context*, void const*, size_t);
    void (*__digest)(Sha2Context*, void*);
};

/**
 * Initialize a SHA-2 hashing context
 *
 * @param context the context to initialize
 * @param algorithm the SHA-2 algorithm to use
 */
void sha2_init(Sha2Context* context, Sha2Algorithm algorithm);

/**
 * Add data to a SHA-2 hashing context
 *
 * @param context the context to update
 * @param data what to put in the hash
 * @param len the number of bytes in DATA
 */
void sha2_update(Sha2Context* context, void const* data, size_t len);

/**
 * Calculate the hash of a SHA-2 context and invalidate it
 *
 * @param context the context to digest
 * @param output the destination if the hash, must be able to hold at least
 * <ALGORITHM>_OUT_SIZE
 */
void sha2_digest(Sha2Context* context, void* output);

CPP_END

#endif // SHA2_H
