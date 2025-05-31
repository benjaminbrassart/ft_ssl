/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 14:18:05 by bbrassar          #+#    #+#             */
/*   Updated: 2025/05/31 14:21:30 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft.h"

#include <stdint.h>

void *ft_memmove(void *dst, void const *src, size_t len)
{
	uint8_t *d = dst;
	uint8_t const *s = src;

	if (src > dst) {
		for (size_t i = 0; i < len; i += 1) {
			d[i] = s[i];
		}
	} else {
		for (size_t i = 0; i < len; i += 1) {
			d[len - i - 1] = s[len - i - 1];
		}
	}

	return dst;
}
