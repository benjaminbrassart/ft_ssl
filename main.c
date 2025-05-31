/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 12:38:31 by bbrassar          #+#    #+#             */
/*   Updated: 2025/05/31 14:32:30 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.h"
#include "hash.h"

#include "libft/ft.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define sizeof_array(Arr) (sizeof(Arr) / sizeof((Arr)[0]))

struct command {
	char const *name;
	int (*executor)(struct arg_iterator *it);
};

struct command_group {
	char const *name;
	char const *flags;
	struct command const *commands;
	size_t command_count;
};

static struct command const HASH_COMMANDS[] = {
	{ "md5", command_md5 },	      { "sha224", command_sha224 },
	{ "sha256", command_sha256 }, { "sha384", command_sha384 },
	{ "sha512", command_sha512 },
};

static struct command_group const COMMAND_GROUPS[] = {
	{
		.name = "Hash",
		.flags = "-p -q -r -s",
		.commands = HASH_COMMANDS,
		.command_count = sizeof_array(HASH_COMMANDS),
	},
};

static struct command const *get_command(char const command_name[])
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

static void print_usage(void)
{
	for (size_t i = 0; i < sizeof_array(COMMAND_GROUPS); i += 1) {
		struct command_group const *group = &COMMAND_GROUPS[i];

		printf("%s commands:\n", group->name);
		for (size_t j = 0; j < group->command_count; j += 1) {
			struct command const *command = &group->commands[j];

			printf("  %s\n", command->name);
		}
		printf("\n");
		printf("  Flags: %s\n", group->flags);
	}
}

static int run_interactive(void)
{
	return EXIT_SUCCESS;
}

int main(int argc, char const *argv[])
{
	struct arg_iterator it;

	argit_init(&it, argc, argv);
	argit_advance(&it); // skip argv[0]

	char const *command_name = argit_next(&it);

	if (command_name == NULL) {
		return run_interactive();
	}

	struct command const *command = get_command(command_name);

	if (command != NULL) {
		return command->executor(&it);
	}

	fprintf(stderr, "ft_ssl: error: '%s' is an invalid command.\n\n",
		command_name);
	print_usage();
	return EXIT_FAILURE;
}
