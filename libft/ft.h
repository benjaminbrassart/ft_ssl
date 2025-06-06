/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 14:15:02 by bbrassar          #+#    #+#             */
/*   Updated: 2025/06/02 18:23:23 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>

int ft_strcmp(char const s1[], char const s2[]);

size_t ft_strlen(char const str[]);

size_t ft_strnlen(char const str[], size_t max_len);

void *ft_memmove(void *dst, void const *src, size_t len);
