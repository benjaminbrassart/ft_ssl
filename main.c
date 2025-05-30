/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 12:38:31 by bbrassar          #+#    #+#             */
/*   Updated: 2025/05/30 18:46:42 by bbrassar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.h"
#include "hash.h"
#include "md5.h"
#include "sha2.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define sizeof_array(Arr) (sizeof(Arr) / sizeof((Arr)[0]))

struct command {
	char const *name;
	int (*executor)(struct arg_iterator *it);
};

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
				 struct hash_options const *opts, FILE *stream,
				 char const *name)
{
	struct hash_algorithm const *alg = cmd->algorithm;
	void *ctx = cmd->context;
	uint8_t *digest = cmd->digest;
	uint8_t rbuf[64];

	if (!opts->quiet && opts->echo && stream == stdin) {
		printf("(\"");
	}

	alg->init(ctx);
	while (!feof(stream) && !ferror(stream)) {
		size_t rc = fread(rbuf, 1, sizeof(rbuf), stream);

		if (rc == 0) {
			break;
		}

		// TODO chomp the terminating EOL
		if (opts->echo && stream == stdin) {
			printf("%.*s", (int)rc, rbuf);
		}

		alg->update(ctx, rbuf, rc);
	}

	if (ferror(stream)) {
		return EXIT_FAILURE;
	}

	if (!opts->quiet && opts->echo && stream == stdin) {
		printf("\")= ");
	}

	alg->digest(ctx, digest);

	if (!opts->quiet && !opts->reverse) {
		if (stream == stdin) {
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

	if (stream != stdin && opts->reverse && !opts->quiet) {
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
		FILE *file = fopen(input->value, "r");

		if (file == NULL) {
			return EXIT_FAILURE;
		}

		res = command_hash_run_file(cmd, opts, file, input->value);
		fclose(file);
		return res;
	}

	case HASH_STDIN:
		return command_hash_run_file(cmd, opts, stdin, "<stdin>");

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
	void *context;
	uint8_t *digest;
	char *digest_str;
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

static int command_md5(struct arg_iterator *it)
{
	return command_hash_generic(it, &ALGORITHM_MD5);
}

static int command_sha224(struct arg_iterator *it)
{
	return command_hash_generic(it, &ALGORITHM_SHA224);
}

static int command_sha256(struct arg_iterator *it)
{
	return command_hash_generic(it, &ALGORITHM_SHA256);
}

static int command_sha384(struct arg_iterator *it)
{
	return command_hash_generic(it, &ALGORITHM_SHA384);
}

static int command_sha512(struct arg_iterator *it)
{
	return command_hash_generic(it, &ALGORITHM_SHA512);
}

static struct command const HASH_COMMANDS[] = {
	{ "md5", command_md5 },	      { "sha224", command_sha224 },
	{ "sha256", command_sha256 }, { "sha384", command_sha384 },
	{ "sha512", command_sha512 },
};

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

	for (size_t i = 0; i < sizeof_array(HASH_COMMANDS); i += 1) {
		struct command const *command = &HASH_COMMANDS[i];

		if (strcmp(command_name, command->name) == 0) {
			return command->executor(&it);
		}
	}

	// TODO print unknown command message
	return EXIT_FAILURE;
}
