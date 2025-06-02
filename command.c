/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:03:51 by bbrassar          #+#    #+#             */
/*   Updated: 2025/06/02 18:07:42 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "command.h"
#include "cipher/cipher.h"
#include "hash/hash.h"

#include "libft/ft.h"

#include <stddef.h>

#include <unistd.h>

#define sizeof_array(Arr) (sizeof(Arr) / sizeof((Arr)[0]))

static struct command const HASH_COMMANDS[] = {
	{ "md5", command_md5 },	      { "sha224", command_sha224 },
	{ "sha256", command_sha256 }, { "sha384", command_sha384 },
	{ "sha512", command_sha512 },
};

static struct command const CIPHER_COMMANDS[] = {
	{ "base64", command_base64 },
};

static struct command_group const COMMAND_GROUPS[] = {
	{
		.name = "Message Digest",
		.flags = "-p -q -r -s",
		.commands = HASH_COMMANDS,
		.command_count = sizeof_array(HASH_COMMANDS),
	},
	{
		.name = "Cipher",
		.flags = NULL,
		.commands = CIPHER_COMMANDS,
		.command_count = sizeof_array(CIPHER_COMMANDS),
	},
};

struct command const *get_command(char const command_name[])
{
	for (size_t i = 0; i < sizeof_array(COMMAND_GROUPS); i += 1) {
		struct command_group const *group = &COMMAND_GROUPS[i];

		for (size_t j = 0; j < group->command_count; j += 1) {
			struct command const *command = &group->commands[j];

			if (ft_strcmp(command->name, command_name) == 0) {
				return command;
			}
		}
	}

	return NULL;
}

void print_available_commands(void)
{
	for (size_t i = 0; i < sizeof_array(COMMAND_GROUPS); i += 1) {
		struct command_group const *group = &COMMAND_GROUPS[i];

		write(STDOUT_FILENO, group->name, ft_strlen(group->name));
		write(STDOUT_FILENO, " commands:\n", 11);

		for (size_t j = 0; j < group->command_count; j += 1) {
			struct command const *command = &group->commands[j];

			write(STDOUT_FILENO, "  ", 2);
			write(STDOUT_FILENO, command->name,
			      ft_strlen(command->name));
			write(STDOUT_FILENO, "\n", 2);
		}

		if (group->flags != NULL) {
			write(STDOUT_FILENO, "\n  Flags: ", 10);
			write(STDOUT_FILENO, group->flags,
			      ft_strlen(group->flags));
			write(STDOUT_FILENO, "\n", 1);
		}
		write(STDOUT_FILENO, "\n", 1);
	}
}
