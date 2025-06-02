/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cipher.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 18:03:53 by bbrassar          #+#    #+#             */
/*   Updated: 2025/06/02 18:40:54 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cipher/cipher.h"

#include "args.h"
#include "cipher/base64.h"
#include "libft/ft.h"
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>

struct file {
	int fd;
	char const *name;
};

int encode_callback(void *ctx, char const data[4])
{
	struct file const *file = ctx;
	size_t len = ft_strnlen(data, 4);

	// TODO add some buffering: this is at best suboptimal
	if (write(file->fd, data, len) < 0) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

static int command_base64_encode(struct file const *in, struct file const *out)
{
	struct base64_encoder enc;
	uint8_t buffer[42];

	base64_enc_init(&enc, (void *)out, encode_callback);
	for (;;) {
		ssize_t rc = read(in->fd, buffer, sizeof(buffer));

		if (rc < 0) {
			return EXIT_FAILURE;
		}

		if (rc == 0) {
			break;
		}

		int res = base64_enc_update(&enc, buffer, (size_t)rc);

		if (res != EXIT_SUCCESS) {
			return res;
		}
	}

	int res = base64_enc_finalize(&enc, 1);

	if (res != EXIT_SUCCESS) {
		return res;
	}

	if (write(out->fd, "\n", 1) < 0) {
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

static int command_base64_decode(struct file const *in, struct file *out)
{
	// TODO implement me
	return EXIT_FAILURE;
}

int command_base64(struct arg_iterator *it)
{
	struct file in = { STDIN_FILENO, NULL };
	struct file out = { STDOUT_FILENO, NULL };
	enum { BASE64_ENCODE, BASE64_DECODE } mode = BASE64_ENCODE;

	char const *arg;

	while ((arg = argit_next(it)) != NULL) {
		if (ft_strcmp(arg, "-i") == 0) {
			char const *file_name = argit_next(it);

			if (file_name == NULL) {
				// TODO print error, expected argument
				return EXIT_FAILURE;
			}

			in.name = file_name;
		} else if (ft_strcmp(arg, "-o") == 0) {
			char const *file_name = argit_next(it);

			if (file_name == NULL) {
				// TODO print error, expected argument
				return EXIT_FAILURE;
			}

			out.name = file_name;
		} else if (ft_strcmp(arg, "-e") == 0) {
			mode = BASE64_ENCODE;
		} else if (ft_strcmp(arg, "-d") == 0) {
			mode = BASE64_DECODE;
		} else {
			// TODO print error, unknown argument
			return EXIT_FAILURE;
		}
	}

	int result = EXIT_FAILURE;

	do {
		if (in.name != NULL) {
			in.fd = open(in.name, O_RDONLY | O_ASYNC);
			if (in.fd < 0) {
				// TODO print error, failed to open input file
				break;
			}
		} else {
			in.name = "<stdin>";
		}

		if (out.name != NULL) {
			out.fd = open(out.name,
				      O_WRONLY | O_CREAT | O_TRUNC | O_ASYNC,
				      0644);
			if (out.fd < 0) {
				// TODO print error, failed to open output file
				break;
			}
		} else {
			out.name = "<stdout>";
		}

		if (mode == BASE64_ENCODE) {
			result = command_base64_encode(&in, &out);
		} else {
			result = command_base64_decode(&in, &out);
		}
	} while (0);

	if (in.fd != -1 && in.fd != STDIN_FILENO) {
		close(in.fd);
	}

	if (out.fd != -1 && out.fd != STDOUT_FILENO) {
		if (close(out.fd) < 0) {
			// TODO print error, failed to close
			result = EXIT_FAILURE;
		}
	}

	return result;
}
