/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hex.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/24 01:02:43 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/24 01:05:50 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEX_H
# define HEX_H

/**
 * @file hex.h
 */

# include <stddef.h>

/**
 * Convert raw bytes to their hexadecimal representation
 *
 * @param dst the destination buffer, must hold at least N * 2 + 1 bytes
 * @param src the source bytes
 * @param n the number of bytes to convert from SRC to DST
 *
 * @return dst
 */
char* memtox(char* dst, void const* src, size_t n);

#endif // HEX_H
