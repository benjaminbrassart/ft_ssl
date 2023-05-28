/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   byteswap.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/28 06:43:47 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/28 06:53:37 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BYTESWAP_H
# define BYTESWAP_H

# include "ft_ssl/cpp.h"

CPP_BEGIN

__uint128_t ft_bswap_128(__uint128_t bytes);

CPP_END

#endif // BYTESWAP_H
