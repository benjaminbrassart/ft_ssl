/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha22.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 23:38:15 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/24 23:57:07 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHA22_HPP
# define SHA22_HPP

# include <stddef.h>
# include <stdint.h>

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
};

#endif // SHA22_HPP
