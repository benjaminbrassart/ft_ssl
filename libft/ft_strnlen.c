/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnlen.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 18:22:49 by bbrassar          #+#    #+#             */
/*   Updated: 2025/06/02 18:23:17 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft.h"

size_t ft_strnlen(char const str[], size_t max_len)
{
	size_t len = 0;

	while (len < max_len && str[len] != '\0') {
		len += 1;
	}

	return len;
}
