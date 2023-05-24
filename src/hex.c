/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hex.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 01:10:12 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/24 03:32:30 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl/hex.h"
#include <stddef.h>
#include <stdint.h>

static char const BASE_HEX[16] = "0123456789abcdef";

char* memtox(char* dst, void const* src, size_t n)
{
    uint8_t const* bytes = (uint8_t const*)src;

    for (size_t i = 0; i < n; ++i)
    {
        dst[i * 2] = BASE_HEX[(bytes[i] >> 4) & 0xF];
        dst[i * 2 + 1] = BASE_HEX[(bytes[i]) & 0xF];
    }
    dst[n * 2] = '\0';
    return dst;
}
