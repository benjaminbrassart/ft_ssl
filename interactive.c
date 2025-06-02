/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/31 16:43:41 by bbrassar          #+#    #+#             */
/*   Updated: 2025/06/02 12:29:10 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "interactive.h"
#include "args.h"
#include "command.h"
#include "verbose.h"

#include "libft/ft.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <readline/history.h>
#include <readline/readline.h>

struct interactive {
	unsigned running : 1;
};

struct splitter {
	char const *line;
	size_t index;
	size_t size;
};

struct word {
	char const *str;
	size_t length;
};

void splitter_skip_spaces(struct splitter *sp)
{
	for (;;) {
		if (sp->index >= sp->size) {
			break;
		}

		if (sp->line[sp->index] != ' ') {
			break;
		}

		sp->index += 1;
	}
}

static unsigned is_quote_char(char c)
{
	return c == '\'' || c == '"';
}

static unsigned is_word_char(char c)
{
	return !(is_quote_char(c) || c == ' ');
}

static int splitter_next_word(struct splitter *sp, struct word *w)
{
	splitter_skip_spaces(sp);

	if (sp->index >= sp->size) {
		w->length = 0;
		w->str = NULL;
		return EXIT_SUCCESS;
	}

	if (is_quote_char(sp->line[sp->index])) {
		char const c = sp->line[sp->index];
		size_t offset = 1;

		while (sp->index + offset < sp->size &&
		       sp->line[sp->index + offset] != c) {
			offset += 1;
		}

		if (sp->index + offset >= sp->size) {
			write(STDERR_FILENO,
			      "ft_ssl: error: missing matching quote\n", 38);
			return EXIT_FAILURE;
		}

		w->str = &sp->line[sp->index + 1];
		w->length = offset - 1;
		sp->index += offset;

		if (sp->index + 1 < sp->size &&
		    sp->line[sp->index + 1] != ' ') {
			write(STDERR_FILENO,
			      "ft_ssl: error: trailing character after quote\n",
			      46);
			return EXIT_FAILURE;
		}
	} else {
		size_t offset = 1;

		while (sp->index + offset < sp->size &&
		       is_word_char(sp->line[sp->index + offset])) {
			offset += 1;
		}

		w->str = &sp->line[sp->index];
		w->length = offset;
		sp->index += offset;

		if (sp->index < sp->size && sp->line[sp->index] != ' ') {
			write(STDERR_FILENO,
			      "ft_ssl: error: trailing quote after character\n",
			      46);
			return EXIT_FAILURE;
		}
	}

	sp->index += 1;
	return EXIT_SUCCESS;
}

// int const a
// int const *a
// int *const a

static char const **split_line(char const line[])
{
	struct splitter sp = {
		.line = line,
		.index = 0,
		.size = ft_strlen(line),
	};
	size_t words = 0;
	size_t total_length = 0;

	for (;;) {
		struct word w;

		if (splitter_next_word(&sp, &w) != EXIT_SUCCESS) {
			return NULL;
		}

		if (w.str == NULL) {
			break;
		}

		words += 1;
		total_length += w.length + 1;
	}

	size_t ptrs_size = (words + 1) * sizeof(char *);
	size_t alloc_size = ptrs_size + total_length;

	char const **ptrs = verbose_malloc(alloc_size);
	char *next_string = (char *)(((uint8_t *)ptrs) + ptrs_size);

	if (ptrs == NULL) {
		return NULL;
	}

	sp.index = 0;

	for (size_t i = 0; i < words; i += 1) {
		struct word w;

		if (splitter_next_word(&sp, &w) != EXIT_SUCCESS) {
			return NULL;
		}

		if (w.str == NULL) {
			break;
		}

		ft_memmove(next_string, w.str, w.length);
		next_string[w.length] = '\0';
		ptrs[i] = next_string;
		next_string += w.length + 1;
	}

	ptrs[words] = NULL;
	return ptrs;
}

static void print_invalid_command(char const command_name[])
{
	write(STDERR_FILENO, "ft_ssl: error: '", 16);
	write(STDERR_FILENO, command_name, ft_strlen(command_name));
	write(STDERR_FILENO,
	      "' is an invalid command. Type 'help' to see available commands.\n",
	      64);
}

static unsigned process_args(char const *argv[])
{
	int argc = 0;

	while (argv[argc] != NULL) {
		argc += 1;
	}

	struct arg_iterator it;

	argit_init(&it, argc, argv);

	char const *command_name = argit_next(&it);

	if (command_name != NULL) {
		if (ft_strcmp(command_name, "exit") == 0) {
			return 0;
		} else if (ft_strcmp(command_name, "help") == 0) {
			print_available_commands();
		} else {
			struct command const *cmd = get_command(command_name);

			if (cmd == NULL) {
				print_invalid_command(command_name);
				return 1;
			}

			cmd->executor(&it);
		}
	}

	return 1;
}

static unsigned process_line(char const line[])
{
	char const **argv = split_line(line);

	if (argv == NULL) {
		return 1;
	}

	unsigned ret = process_args(argv);

	free(argv);

	return ret;
}

int run_interactive(void)
{
	char *line;

	while ((line = readline("ft_ssl> ")) != NULL) {
		unsigned running = process_line(line);
		add_history(line);
		free(line);
		if (!running) {
			break;
		}
	}

	return EXIT_SUCCESS;
}
