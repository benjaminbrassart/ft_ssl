/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotate.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/25 21:09:08 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/25 21:18:54 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROTATE_H
# define ROTATE_H

# include "ft_ssl/cpp.h"
# include <stdint.h>

CPP_BEGIN

uint32_t rotate_left_u32(uint32_t n, uint32_t r);
uint32_t rotate_right_u32(uint32_t n, uint32_t r);

uint64_t rotate_left_u64(uint64_t n, uint64_t r);
uint64_t rotate_right_u64(uint64_t n, uint64_t r);

CPP_END

#endif // ROTATE_H
