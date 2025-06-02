/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verbose.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 12:27:00 by bbrassar          #+#    #+#             */
/*   Updated: 2025/06/02 12:27:40 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/ft.h"

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

void *verbose_malloc(size_t n)
{
	void *r = malloc(n);

	if (r == NULL) {
		char const *emsg = strerror(errno);

		write(STDERR_FILENO,
		      "ft_ssl: error: failed to allocate memory: ", 42);
		write(STDERR_FILENO, emsg, ft_strlen(emsg));
		write(STDERR_FILENO, "\n", 1);
	}

	return r;
}

int verbose_open(char const file_name[], int flags)
{
	int fd = open(file_name, flags);

	if (fd < 0) {
		char const *emsg = strerror(errno);

		write(STDERR_FILENO, "ft_ssl: error: failed to open ", 30);
		write(STDERR_FILENO, file_name, strlen(file_name));
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, emsg, ft_strlen(emsg));
		write(STDERR_FILENO, "\n", 1);
	}

	return fd;
}

ssize_t verbose_read(int fd, void *buffer, size_t len, char const file_name[])
{
	ssize_t rc = read(fd, buffer, len);

	if (rc < 0) {
		char const *emsg = strerror(errno);

		write(STDERR_FILENO, "ft_ssl: error: failed to read from ", 35);
		write(STDERR_FILENO, file_name, strlen(file_name));
		write(STDERR_FILENO, ": ", 2);
		write(STDERR_FILENO, emsg, ft_strlen(emsg));
		write(STDERR_FILENO, "\n", 1);
	}

	return rc;
}
