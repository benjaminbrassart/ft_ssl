/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 21:11:12 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/25 21:15:36 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl/rotate.h"
#include <stdint.h>

uint32_t rotate_left_u32(uint32_t n, uint32_t r)
{
    return (n << r) | (n >> (32 - r));
}

uint32_t rotate_right_u32(uint32_t n, uint32_t r)
{
    return (n >> r) | (n << (32 - r));
}

uint64_t rotate_left_u64(uint64_t n, uint64_t r)
{
    return (n << r) | (n >> (64 - r));
}
uint64_t rotate_right_u64(uint64_t n, uint64_t r)
{
    return (n >> r) | (n << (64 - r));
}
