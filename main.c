/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 12:38:31 by bbrassar          #+#    #+#             */
/*   Updated: 2025/05/31 18:08:42 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.h"
#include "command.h"

#include "interactive.h"
#include "libft/ft.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

static void print_invalid_command(char const command_name[])
{
	write(STDERR_FILENO, "ft_ssl: error: '", 16);
	write(STDERR_FILENO, command_name, ft_strlen(command_name));
	write(STDERR_FILENO, "' is an invalid command.\n", 26);
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

	print_invalid_command(command_name);
	write(STDERR_FILENO, "\n", 1);
	print_available_commands();

	return EXIT_FAILURE;
}
