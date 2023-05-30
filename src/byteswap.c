/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   byteswap.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 06:49:56 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/28 06:53:33 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl/byteswap.h"
#include "libft/ft_byteswap.h"

__uint128_t ft_bswap_128(__uint128_t bytes)
{
	return (((__uint128_t)ft_bswap_64(bytes)) << 64 | ft_bswap_64(bytes >> 64));
}
