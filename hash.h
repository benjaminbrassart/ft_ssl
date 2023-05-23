/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 22:49:09 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/24 00:47:36 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASH_H
# define HASH_H

# include <stddef.h>
# include <stdint.h>

# define HASHOPT_PRINT_STDIN (1 << 0)
# define HASHOPT_QUIET (1 << 1)
# define HASHOPT_REVERSE (1 << 2)

typedef enum hash_input_type
{
    InputString,
    InputFile,
    InputStdin,
} HashInputType;

typedef struct hash_input
{
    HashInputType type;
    char const* value;
} HashInput;

typedef struct hash_options
{
    HashInput* inputs;
    size_t inputs_size;
    uint32_t bits;
} HashOptions;

int hash_options_parse(HashOptions* options, int argc, char const* argv[]);
void hash_options_cleanup(HashOptions* options);

#endif // HASH_H
