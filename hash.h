/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 12:41:58 by bbrassar          #+#    #+#             */
/*   Updated: 2025/05/30 19:34:03 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>
#include <stdint.h>

typedef void(hash_init_t)(void *ctx);
typedef void(hash_update_t)(void *ctx, void const *data, size_t len);
typedef void(hash_digest_t)(void *ctx, uint8_t *digest);

struct hash_algorithm {
	char const *name_pretty;
	hash_init_t *init;
	hash_update_t *update;
	hash_digest_t *digest;
	size_t digest_size;
	size_t context_size;
};
