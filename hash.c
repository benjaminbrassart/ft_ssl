/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 23:23:16 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/24 01:34:09 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int hash_options_parse(HashOptions* options, int argc, char const* argv[])
{
    options->bits = 0;

    options->inputs = malloc(sizeof (*options->inputs) * argc);

    if (options->inputs == NULL)
    {
        int err = errno;

        fprintf(stderr, "ft_ssl: %s (%d)\n", strerror(err), err);
        goto _error;
    }

    if (!isatty(STDIN_FILENO))
    {
        options->inputs[options->inputs_size].type = InputStdin;
        options->inputs[options->inputs_size].value = "stdin";
        options->inputs_size += 1;
    }

    int i = 1;

    for (; i < argc; i += 1)
    {
        if (argv[i][0] != '-' || (argv[i][0] != '\0' && argv[i][1] == '\0') || (argv[i][1] == '-' && argv[i][2] == '\0'))
            break;
        switch (argv[i][1])
        {
            case 'p':
                options->bits &= HASHOPT_PRINT_STDIN;
                break;
            case 'q':
                options->bits &= HASHOPT_QUIET;
                break;
            case 'r':
                options->bits &= HASHOPT_REVERSE;
                break;
            case 's':
                i += 1;
                if (i >= argc)
                {
                    fprintf(stderr, "ft_ssl: options requires an argument -- 's'\n");
                    goto _error;
                }
                options->inputs[options->inputs_size].type = InputString;
                options->inputs[options->inputs_size].value = argv[i];
                options->inputs_size += 1;
                break;
            default:
                fprintf(stderr, "ft_ssl: unknown option -- '%c'\n", argv[i][1]);
                goto _error;
        }
    }

    for(; i < argc; i += 1)
    {
        options->inputs[options->inputs_size].type = InputFile;
        options->inputs[options->inputs_size].value = argv[i];
        options->inputs_size += 1;
    }

    if (options->inputs_size == 0)
    {
        options->inputs[options->inputs_size].type = InputStdin;
        options->inputs[options->inputs_size].value = "stdin";
        options->inputs_size += 1;
    }

    return EXIT_SUCCESS;

_error:
    hash_options_cleanup(options);
    return EXIT_FAILURE;
}

void hash_options_cleanup(HashOptions* options)
{
    free(options->inputs);
}
