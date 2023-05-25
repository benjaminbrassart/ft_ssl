/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha2.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 23:38:15 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/25 06:24:04 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHA2_HPP
# define SHA2_HPP

# include "ft_ssl/cpp.h"
# include <stddef.h>
# include <stdint.h>

CPP_BEGIN

# define SHA224_OUT_SIZE (28)
# define SHA256_OUT_SIZE (32)
# define SHA384_OUT_SIZE (48)
# define SHA512_OUT_SIZE (64)

typedef enum sha2_algorithm Sha2Algorithm;

typedef struct sha2_context Sha2Context;

typedef struct sha224_data Sha224Data;
typedef struct sha256_data Sha256Data;
typedef struct sha384_data Sha384Data;
typedef struct sha512_data Sha512Data;

enum sha2_algorithm
{
    SHA224,
    SHA256,
    SHA384,
    SHA512,
};

struct sha224_data
{
    uint32_t hash_vars[8];
    union
    {
        uint8_t u8[64];
        uint32_t u32[16];
    } buffer;
};

struct sha256_data
{
    uint32_t hash_vars[8];
    union
    {
        uint8_t u8[64];
        uint32_t u32[16];
    } buffer;
};

struct sha384_data
{
    uint64_t hash_vars[8];
    union
    {
        uint8_t u8[128];
        uint64_t u64[32];
    } buffer;
};

struct sha512_data
{
    uint64_t hash_vars[8];
    union
    {
        uint8_t u8[128];
        uint64_t u64[32];
    } buffer;
};

/**
 * An execution context for the SHA-2 algorithm family
 *
 * @param alg the algorithm used for this context
 * @param length the total length of the input
 * @param data algorithm-dependent data, such as the buffer and internal state (hash variables)
 */
struct sha2_context
{
    Sha2Algorithm alg;
    uint64_t length;
    union
    {
        Sha224Data sha224;
        Sha256Data sha256;
        Sha384Data sha384;
        Sha512Data sha512;
    } data;
    void (*__update)(Sha2Context*, void const*, size_t);
    void (*__digest)(Sha2Context*, void*);
};

void sha2_init(Sha2Context* context, Sha2Algorithm algorithm);

void sha2_update(Sha2Context* context, void const* data, size_t len);

void sha2_digest(Sha2Context* context, void* output);

CPP_END

#endif // SHA2_HPP
