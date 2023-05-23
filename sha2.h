/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha2.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 17:26:33 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/23 17:40:03 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file sha2.h
 */

#ifndef SHA2_H
# define SHA2_H

# include "cpp.h"
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

typedef struct sha2_context
{
    Sha2Algorithm alg;
    uint32_t hash_vars[8];
    uint8_t buffer[64];
    uint64_t length;
} Sha2Context;

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
void sha2_update(Sha2Algorithm* context, void const* data, size_t len);

/**
 * Calculate the hash of a SHA-2 context and invalidate it
 *
 * @param context the context to digest
 * @param output the destination if the hash, must be able to hold at least
 * <ALGORITHM>_OUT_SIZE
 */
void sha2_digest(Sha2Algorithm* context, void* output);

CPP_END

#endif // SHA2_H
