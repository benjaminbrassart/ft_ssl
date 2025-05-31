/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 22:29:21 by bbrassar          #+#    #+#             */
/*   Updated: 2025/05/31 14:13:22 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hash.h"
#include "args.h"
#include "md5.h"
#include "sha2.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>

enum hash_input_type {
	HASH_FILE,
	HASH_STRING,
	HASH_STDIN,
};

struct hash_input {
	enum hash_input_type type;

	/// Depending on type:
	///   - HASH_FILE: a path to the file
	///   - HASH_STRING: the actual value of the string
	///   - HASH_STDIN: ignored
	char const *value;

	struct hash_input *next;
};

struct hash_input_list {
	struct hash_input *begin;
	struct hash_input *end;
	size_t size;
};

struct hash_options {
	unsigned echo : 1;
	unsigned quiet : 1;
	unsigned reverse : 1;
	struct hash_input_list inputs;
};

struct hash_command {
	struct hash_algorithm const *algorithm;
	void *context;
	uint8_t *digest;
	char *digest_str;
};

static void free_inputs(struct hash_input_list *list)
{
	struct hash_input *it = list->begin;

	while (it != NULL) {
		struct hash_input *next = it->next;

		free(it);
		it = next;
	}

	list->begin = NULL;
	list->end = NULL;
	list->size = 0;
}

static int append_input(struct hash_input_list *list, enum hash_input_type type,
			char const *value)
{
	struct hash_input *input;

	input = malloc(sizeof(*input));
	if (input == NULL) {
		// TODO print message, malloc failed
		return EXIT_FAILURE;
	}

	if (list->size == 0) {
		list->begin = input;
	} else {
		list->end->next = input;
	}

	input->next = NULL;
	input->type = type;
	input->value = value;

	list->end = input;
	list->size += 1;

	return EXIT_SUCCESS;
}

static int prepend_input(struct hash_input_list *list,
			 enum hash_input_type type, char const *value)
{
	struct hash_input *input;

	input = malloc(sizeof(*input));
	if (input == NULL) {
		// TODO print message, malloc failed
		return EXIT_FAILURE;
	}

	if (list->size == 0) {
		list->end = input;
	}

	input->type = type;
	input->value = value;
	input->next = list->begin;

	list->begin = input;
	list->size += 1;

	return EXIT_SUCCESS;
}

static int command_hash_run_file(struct hash_command *cmd,
				 struct hash_options const *opts, int fd,
				 char const *name)
{
	struct hash_algorithm const *alg = cmd->algorithm;
	void *ctx = cmd->context;
	uint8_t *digest = cmd->digest;
	uint8_t rbuf[64];

	if (!opts->quiet && opts->echo && fd == STDIN_FILENO) {
		printf("(\"");
	}

	alg->init(ctx);
	for (;;) {
		ssize_t rc = read(fd, rbuf, sizeof(rbuf));

		if (rc == 0) {
			break;
		}

		if (rc < 0) {
			// TODO print message, read failed
			return EXIT_FAILURE;
		}

		alg->update(ctx, rbuf, (size_t)rc);

		size_t i = 0;

		// ignore \r and \n for printing
		while (i < (size_t)rc) {
			if (rbuf[i] == '\n' || rbuf[i] == '\r') {
				memmove(&rbuf[i], &rbuf[i + 1], (size_t)rc - i);
				rc -= 1;
			} else {
				i += 1;
			}
		}

		if (opts->echo && fd == STDIN_FILENO) {
			printf("%.*s", (int)rc, rbuf);
		}
	}

	if (opts->echo && fd == STDIN_FILENO) {
		if (opts->quiet) {
			printf("\n");
		} else {
			printf("\")= ");
		}
	}

	alg->digest(ctx, digest);

	if (!opts->quiet && !opts->reverse) {
		if (fd == STDIN_FILENO) {
			if (!opts->echo) {
				printf("(stdin)= ");
			}
		} else {
			printf("%s (%s) = ", alg->name_pretty, name);
		}
	}

	for (size_t i = 0; i < alg->digest_size; i += 1) {
		printf("%02x", cmd->digest[i]);
	}

	if (fd != STDIN_FILENO && opts->reverse && !opts->quiet) {
		printf(" %s", name);
	}

	printf("\n");

	return EXIT_SUCCESS;
}

static int command_hash_run_string(struct hash_command *cmd,
				   struct hash_options const *opts,
				   char const *str)
{
	struct hash_algorithm const *alg = cmd->algorithm;
	void *ctx = cmd->context;
	uint8_t *digest = cmd->digest;

	alg->init(ctx);
	alg->update(ctx, str, strlen(str));
	alg->digest(ctx, digest);

	if (!opts->quiet && !opts->reverse) {
		printf("%s (\"%s\") = ", alg->name_pretty, str);
	}

	for (size_t i = 0; i < alg->digest_size; i += 1) {
		printf("%02x", cmd->digest[i]);
	}

	if (!opts->quiet && opts->reverse) {
		printf(" \"%s\"", str);
	}

	printf("\n");
	return EXIT_SUCCESS;
}

static int command_hash_run_input(struct hash_command *cmd,
				  struct hash_options const *opts,
				  struct hash_input const *input)
{
	switch (input->type) {
	case HASH_FILE: {
		int res;
		int fd = open(input->value, O_RDONLY | O_ASYNC);

		if (fd == -1) {
			return EXIT_FAILURE;
		}

		res = command_hash_run_file(cmd, opts, fd, input->value);
		close(fd);
		return res;
	}

	case HASH_STDIN:
		return command_hash_run_file(cmd, opts, STDIN_FILENO,
					     "<stdin>");

	case HASH_STRING:
		return command_hash_run_string(cmd, opts, input->value);

	default:
		return EXIT_FAILURE;
	}
}

static int command_hash_run_cmd(struct hash_command *cmd,
				struct hash_options const *opts)
{
	int result = EXIT_SUCCESS;

	for (struct hash_input *it = opts->inputs.begin; it != NULL;
	     it = it->next) {
		int tmp = command_hash_run_input(cmd, opts, it);

		if (tmp > result) {
			result = tmp;
		}
	}

	return result;
}

static int command_hash_parse_opts(struct arg_iterator *it,
				   struct hash_options *opts)
{
	int result = EXIT_SUCCESS;
	char const *arg;

	opts->echo = 0;
	opts->quiet = 0;
	opts->reverse = 0;
	opts->inputs = (struct hash_input_list){
		.begin = NULL,
		.end = NULL,
		.size = 0,
	};

	while (result == EXIT_SUCCESS && (arg = argit_peek(it)) != NULL) {
		if (strcmp(arg, "-p") == 0) {
			opts->echo = 1;
		} else if (strcmp(arg, "-q") == 0) {
			opts->quiet = 1;
		} else if (strcmp(arg, "-r") == 0) {
			opts->reverse = 1;
		} else if (strcmp(arg, "-s") == 0) {
			argit_advance(it);

			char const *str = argit_peek(it);

			if (str == NULL) {
				// TODO print message, missing argument
				// after -s
				result = EXIT_FAILURE;
				break;
			}

			result = append_input(&opts->inputs, HASH_STRING, str);
		} else {
			break;
		}

		argit_advance(it);
	}

	while (result == EXIT_SUCCESS && (arg = argit_next(it)) != NULL) {
		result = append_input(&opts->inputs, HASH_FILE, arg);
	}

	if (result == EXIT_SUCCESS && opts->echo) {
		result = prepend_input(&opts->inputs, HASH_STDIN, NULL);
	}

	if (result == EXIT_SUCCESS && opts->inputs.size == 0) {
		result = append_input(&opts->inputs, HASH_STDIN, NULL);
		if (result != EXIT_SUCCESS) {
			return result;
		}
	}

	if (result != EXIT_SUCCESS) {
		free_inputs(&opts->inputs);
	}

	return result;
}

static int command_hash_generic(struct arg_iterator *it,
				struct hash_algorithm const *algorithm)
{
	void *context = NULL;
	uint8_t *digest = NULL;
	char *digest_str = NULL;
	struct hash_options opts;
	int result;

	result = command_hash_parse_opts(it, &opts);
	if (result != EXIT_SUCCESS) {
		return result;
	}

	result = EXIT_FAILURE;
	do {
		context = malloc(algorithm->context_size);
		if (context == NULL) {
			break;
		}

		digest = malloc(algorithm->digest_size);
		if (digest == NULL) {
			break;
		}

		digest_str = malloc(algorithm->digest_size * 2 + 1);
		if (digest_str == NULL) {
			break;
		}

		struct hash_command cmd = {
			.algorithm = algorithm,
			.context = context,
			.digest = digest,
			.digest_str = digest_str,
		};

		result = command_hash_run_cmd(&cmd, &opts);
	} while (0);

	free(digest_str);
	free(digest);
	free(context);
	free_inputs(&opts.inputs);
	return result;
}

int command_md5(struct arg_iterator *it)
{
	return command_hash_generic(it, &ALGORITHM_MD5);
}

int command_sha224(struct arg_iterator *it)
{
	return command_hash_generic(it, &ALGORITHM_SHA224);
}

int command_sha256(struct arg_iterator *it)
{
	return command_hash_generic(it, &ALGORITHM_SHA256);
}

int command_sha384(struct arg_iterator *it)
{
	return command_hash_generic(it, &ALGORITHM_SHA384);
}

int command_sha512(struct arg_iterator *it)
{
	return command_hash_generic(it, &ALGORITHM_SHA512);
}
