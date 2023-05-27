/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/26 23:44:30 by bbrassar          #+#    #+#             */
/*   Updated: 2023/05/27 07:00:23 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl/hash.h"
#include "ft_ssl/hex.h"
#include "ft_ssl/md5.h"
#include "ft_ssl/sha2.h"
#include "ft_ssl/command.h"
#include "libft/ft.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct command_lookup
{
    char const* name;
    int (*command)(int, char const*[]);
    void (*help)(void);
} CommandLookup;

typedef struct command_group
{
    char const* name;
    CommandLookup commands[256];
} CommandGroup;

static CommandLookup const* __getcommand(char const* name);
static void __help(void);
static void __help_help(void);
static void __help_md5(void);
static void __help_sha256(void);
static int __command_help(int argc, char const* argv[]);
static int __command_exit();
static int __command_md5(int argc, char const* argv[]);
static int __command_sha256(int argc, char const* argv[]);

static CommandGroup const COMMAND_GROUPS[] = {
    {"Standard commands", {
        {"exit", __command_exit, NULL},
        {"help", __command_help, __help_help},
    }},
    {"Message Digest commands", {
        {"md5", __command_md5, __help_md5},
        {"sha256", __command_sha256, __help_sha256},
    }},
    {"Cipher commands", {
    }},
};

int execute_command(char const* command, int argc, char const** argv)
{
    CommandLookup const* lookup = __getcommand(command);

    if (lookup != NULL)
        return lookup->command(argc, argv);
    fprintf(stderr, "Invalid command '%s'; type \"help\" for a list.\n", command);
    return 127;
}

static int __command_digest(int argc, char const* argv[], void* context, void (*init)(), void (*update)(), void (*digest)(), size_t out_size)
{
    HashOptions options;
    int result = EXIT_SUCCESS;
    char* hash = malloc(out_size * 3 + 1);
    char* hex = hash + out_size;

    if (hash == NULL || hash_options_parse(&options, argc, argv) == EXIT_FAILURE)
    {
        free(hash);
        return EXIT_FAILURE;
    }

    HashInput const* input;

    for (size_t i = 0; i < options.inputs_size; ++i)
    {
        input = &options.inputs[i];
        init(context);

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

                    fprintf(stderr, "%s: %s\n", input->value, strerror(err));
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

                        fprintf(stderr, "%s: %s\n", input->value, strerror(err));
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

                        fprintf(stderr, "%s: %s\n", input->value, strerror(err));
                        result = EXIT_FAILURE;
                        free(read_buffer);
                        goto _break_file;
                    }
                    if (rr == 0)
                        break;
                    update(context, read_buffer, rr);
                }

_break_file:
                if (fd > 0)
                    close(fd);
                free(read_buffer);
                break;
            }
            case InputString:
                for (char const* s = input->value; *s != '\0'; s += 1)
                    update(context, s, 1);
                break;
            default:
                continue;
        }

        digest(context, hash);
        memtox(hex, hash, out_size);
        printf("%s\n", hex);
    }

    free(hash);
    hash_options_cleanup(&options);
    return result;
}

static int __command_md5(int argc, char const* argv[])
{
    Md5Context ctx;

    return __command_digest(argc, argv, &ctx, md5_init, md5_update, md5_digest, MD5_OUT_SIZE);
}

static int __command_sha256(int argc, char const* argv[])
{
    Sha2Context ctx;

    return __command_digest(argc, argv, &ctx, sha256_init, sha2_update, sha2_digest, SHA256_OUT_SIZE);
}

static int __command_help(int argc, char const* argv[])
{
    switch (argc)
    {
        case 1:
            __help();
            break;
        case 2:
            {
                CommandLookup const* lookup = __getcommand(argv[1]);

                if (lookup == NULL)
                {
                    fprintf(stderr, "Invalid command '%s'; type \"help\" for a list.\n", argv[1]);
                    return EXIT_FAILURE;
                }
                if (lookup->help != NULL)
                    lookup->help();
            }
            break;
        default:
            fputs("Usage: help", stderr);
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

static int __command_exit()
{
    return EXIT_FORCE;
}

static void __help_digest(char const* func_name)
{
    printf(
        "Usage: %s [options] [file...]\n" \
        "  file...         files to digest (default is stdin)\n" \
        "\n" \
        "  -p              echo STDIN to STDOUT, and append the checksum to STDOUT\n"
        "  -q              quiet mode\n"
        "  -r              reverse the output\n"
        "  -s <string>     hash the given string\n"
        "",
        func_name
    );
}

static void __help(void)
{
    for (size_t i = 0; i < (sizeof (COMMAND_GROUPS) / sizeof (*COMMAND_GROUPS)); i += 1)
    {
        printf("%s\n", COMMAND_GROUPS[i].name);

        size_t j;

        for (j = 0; COMMAND_GROUPS[i].commands[j].name != NULL; j += 1)
        {
            printf("%-18s ", COMMAND_GROUPS[i].commands[j].name);
            if ((j + 1) % 4 == 0)
                puts("");
        }

        if (j % 4 != 0)
            puts("");
        puts("");
    }
}

static void __help_md5(void)
{
    __help_digest("md5");
}

static void __help_sha256(void)
{
    __help_digest("sha256");
}

static void __help_help(void)
{
    puts("Usage: help [command]");
}

static CommandLookup const* __getcommand(char const* name)
{
    for (size_t i = 0; i < (sizeof (COMMAND_GROUPS) / sizeof (*COMMAND_GROUPS)); i += 1)
        for (size_t j = 0; COMMAND_GROUPS[i].commands[j].name != NULL; j += 1)
            if (ft_strcmp(name, COMMAND_GROUPS[i].commands[j].name) == 0)
                return &COMMAND_GROUPS[i].commands[j];
    return NULL;
}
