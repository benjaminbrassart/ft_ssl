/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interactive.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 23:27:34 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/27 05:49:00 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl/command.h"
#include "libft/ft.h"
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <readline/history.h>
#include <readline/readline.h>

typedef struct string_node StringNode;

struct string_node
{
    char* str;
    StringNode* next;
};

typedef struct string_list
{
    StringNode* begin;
    StringNode* end;
    size_t size;
} StringList;

static void __help(void);
static char** __split_input(char const* line);
static void __free_args(char** args);

int run_interactive(void)
{
    char** args;

    for (;;)
    {
        {
            char* line = readline("ft_ssl> ");

            if (line == NULL)
                return EXIT_SUCCESS;

            add_history(line);
            args = __split_input(line);
            free(line);
        }

        if (args == NULL || args[0] == NULL)
        {
            free(args);
            continue;
        }

        if (ft_strcmp(args[0], "exit") == 0)
        {
            __free_args(args);
            return EXIT_SUCCESS;
        }
        else if (ft_strcmp(args[0], "help") == 0)
        {
            __free_args(args);
            __help();
        }
        else
        {
            int argc = 1;

            while (args[argc] != NULL)
                argc += 1;

            if (execute_command(args[0], argc, (char const**)args) != EXIT_SUCCESS)
                fprintf(stderr, "error in %s\n", args[0]);
        }
    }
}

static void __help(void)
{}

static char** __split_input(char const* line)
{
    char const* s = line;
    char** strings = NULL;
    size_t strings_len = 0;
    size_t strings_cap = 0;
    size_t input_len;
    size_t skip_last;

    while (ft_isspace(*s))
        s += 1;

    for (;;)
    {
        if (ft_isspace(*s))
        {
            strings_len += 1;

            while (ft_isspace(*s))
                s += 1;
        }

        if (*s == '\0')
            break;

        if (*s == '\'' || *s == '\"')
        {
            input_len = 1;
            while (s[input_len] != '\0' && s[input_len] != *s)
                input_len += 1;
            if (s[input_len] == '\0')
            {
                fprintf(stderr, "Error: failed to parse input: quote not closed at index %ld\n", (s - line) + 1);
                goto _error;
            }
            s += 1;
            input_len -= 1;
            skip_last = 1;
        }
        else
        {
            input_len = 0;
            while (!(ft_isspace(s[input_len]) || s[input_len] == '\'' || s[input_len] == '\"' || s[input_len] == '\0'))
                input_len += 1;
            skip_last = 0;
        }

        char* append = ft_strndup(s, input_len);

        if (append == NULL)
            goto _malloc_error;

        if (strings_cap <= strings_len)
        {
            size_t new_cap = strings_cap * 2;

            if (new_cap < 16)
                new_cap = 16;

            char** new_strings = ft_calloc(new_cap + 1, sizeof (*new_strings));

            if (new_strings == NULL)
            {
                free(append);
                goto _malloc_error;
            }

            for (size_t n = 0; n < strings_len; n += 1)
                new_strings[n] = strings[n];

            free(strings);
            strings = new_strings;
            strings_cap = new_cap;
        }

        size_t lengths[2] = {0, 0};

        if (strings[strings_len] != NULL)
            lengths[0] = ft_strlen(strings[strings_len]);
        lengths[1] = input_len;

        char* joined = malloc(sizeof(*joined) * (lengths[0] + lengths[1] + 1));

        if (joined == NULL)
            goto _malloc_error;

        ft_memcpy(joined, strings[strings_len], lengths[0]);
        ft_memcpy(joined + lengths[0], append, lengths[1]);
        joined[lengths[0] + lengths[1]] = '\0';

        free(append);
        free(strings[strings_len]);

        strings[strings_len] = joined;

        s += input_len + skip_last;
    }

    strings_len += 1;
    strings[strings_len] = NULL;

    return strings;

_malloc_error:
    {
        int err = errno;

        fprintf(stderr, "Error: %s (%d)\n", strerror(err), err);
    }

_error:
    for (size_t n = 0; n < strings_len; n += 1)
        free(strings[n]);
    free(strings);
    return NULL;
}

static void __free_args(char** args)
{
    if (args != NULL)
        for (char** it = args; *args != NULL; args += 1)
            free(*it);
    free(args);
}
