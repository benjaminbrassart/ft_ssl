/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 18:06:40 by bbrassar          #+#    #+#             */
/*   Updated: 2025/05/31 18:07:16 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "args.h"

#include <stddef.h>

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

struct command const *get_command(char const command_name[]);

void print_available_commands(void);
