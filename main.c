/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/23 22:35:54 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/24 01:38:38 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash.h"
#include "hex.h"
#include "md5.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define FT_SSL_READ_SIZE (1 << 16)

typedef struct command_lookup
{
    char const* name;
    int (*command)(int, char const*[]);
} CommandLookup;

static int __command_md5(int argc, char const* argv[]);

static CommandLookup const COMMANDS[] = {
    {"md5", __command_md5},
};

int main(int argc, char const* argv[])
{
    if (argc == 1)
    {
        fprintf(stderr, "Usage: ft_ssl <command> [flags] [files...]\n");
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < (sizeof (COMMANDS) / sizeof (*COMMANDS)); ++i)
        if (strcmp(argv[1], COMMANDS[i].name) == 0)
            return COMMANDS[i].command(argc - 1, &argv[1]);

    fprintf(stderr, "ft_ssl: unknown command: %s\n", argv[1]);
    return EXIT_FAILURE;
}

static int __command_md5(int argc, char const* argv[])
{
    HashOptions options;
    int result = EXIT_SUCCESS;

    if (hash_options_parse(&options, argc, argv) == EXIT_FAILURE)
        return EXIT_FAILURE;

    HashInput const* input;
    Md5Context ctx;

    for (size_t i = 0; i < options.inputs_size; ++i)
    {
        input = &options.inputs[i];
        md5_init(&ctx);

        switch (input->type)
        {
            case InputFile:
            case InputStdin:
            {
                int fd = -1;
                void* read_buffer = malloc(FT_SSL_READ_SIZE);

                if (read_buffer == NULL)
                {
                    int err = errno;

                    fprintf(stderr, "ft_ssl: %s: %s (%d)\n", input->value, strerror(err), err);
                    result = EXIT_FAILURE;
                    goto _break_file;
                }

                if (input->type == InputStdin)
                    fd = STDIN_FILENO;
                else
                {
                    fd = open(input->value, O_RDONLY);

                    if (fd == -1)
                    {
                        int err = errno;

                        fprintf(stderr, "ft_ssl: %s: %s (%d)\n", input->value, strerror(err), err);
                        result = EXIT_FAILURE;
                        free(read_buffer);
                        goto _break_file;
                    }
                }

                int rr;

                for (;;)
                {
                    rr = read(fd, read_buffer, FT_SSL_READ_SIZE);
                    if (rr < 0)
                    {
                        int err = errno;

                        fprintf(stderr, "ft_ssl: %s: %s (%d)\n", input->value, strerror(err), err);
                        result = EXIT_FAILURE;
                        free(read_buffer);
                        goto _break_file;
                    }
                    if (rr == 0)
                        break;
                    md5_update(&ctx, read_buffer, rr);
                }

_break_file:
                if (fd > 0)
                    close(fd);
                free(read_buffer);
                break;
            }
            case InputString:
                for (char const* s = input->value; *s != '\0'; s += 1)
                    md5_update(&ctx, s, 1);
                break;
            default:
                continue;
        }

        uint8_t hash[MD5_OUT_SIZE];
        char hex[MD5_OUT_SIZE * 2 + 1];

        md5_digest(&ctx, hash);
        memtox(hex, hash, MD5_OUT_SIZE);
        printf("%s\n", hex);
    }

    hash_options_cleanup(&options);
    return result;
}
