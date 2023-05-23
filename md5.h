/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 16:06:51 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/23 17:31:40 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file md5.h
 */

#ifndef MD5_H
# define MD5_H

# include "cpp.h"
# include <stdint.h>
# include <stddef.h>

CPP_BEGIN

# define MD5_OUT_SIZE (16)

/**
 * @brief An execution context for the MD5 hashing algorithm.
 *
 * @details Must be initialized with md5_init().
 *
 * @param hash_vars A, B, C and D
 * @param buffer the data held, waiting to be digested
 * @param length the total length of the message, modulo 2^64
 */
typedef struct md5_context
{
    uint32_t hash_vars[4];
    uint8_t buffer[64];
    uint64_t length;
} Md5Context;

/**
 * Initialize a MD5 hashing context
 *
 * @param context the context to initialize
*/
void md5_init(Md5Context* context);

/**
 * Add data to a MD5 hashing context
 *
 * @param context the context to update
 * @param data what to put in the hash
 * @param len the number of bytes in DATA
 */
void md5_update(Md5Context* context, void const* data, size_t len);

/**
 * Calculate the hash of a MD5 context and invalidate it
 *
 * @param context the context to digest
 * @param output the destination of the hash, must be able to hold at least
 * MD5_OUT_SIZE bytes
 */
void md5_digest(Md5Context* context, void* output);

CPP_END

#endif // MD5_H
