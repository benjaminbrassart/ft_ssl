/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verbose.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:26:35 by bbrassar          #+#    #+#             */
/*   Updated: 2025/06/02 12:28:02 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>

#include <unistd.h>

void *verbose_malloc(size_t n);

int verbose_open(char const file_name[], int flags);

ssize_t verbose_read(int fd, void *buffer, size_t len, char const file_name[]);
