# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    GNUmakefile                                        :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/30 12:28:05 by bbrassar          #+#    #+#              #
#    Updated: 2025/06/02 13:17:26 by bbrassar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

override MAKEFILE := $(lastword $(MAKEFILE_LIST))

NAME := ft_ssl

override SRC := main.c interactive.c args.c command.c verbose.c \
	hash/hash.c hash/md5.c hash/sha256.c hash/sha512.c \
	cipher/base64.c \
	libft/ft_strcmp.c libft/ft_strlen.c libft/ft_memmove.c
override OBJ := $(SRC:%.c=%.c.o)
override DEP := $(OBJ:.c.o=.c.d)

override TEST_SRC := hash/md5_test.c hash/sha224_test.c hash/sha256_test.c hash/sha384_test.c \
	hash/sha512_test.c cipher/base64_test.c
override TEST_OBJ := $(TEST_SRC:%.c=%.c.o)
override TEST_DEP := $(TEST_OBJ:.c.o=.c.d)
override TEST_BIN := $(TEST_OBJ:.c.o=)

override CFLAGS += -Wall -Wextra -c
override CPPFLAGS += -MMD -MP -I .
override LDLIBS += -lreadline
override LDFLAGS ?=

$(NAME): $(OBJ)
	$(CC) $(LDLIBS) $(LDFLAGS) -o $@ $^

$(OBJ): .EXTRA_PREREQS = $(MAKEFILE)
$(OBJ): %.c.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $<

-include $(DEP)

$(TEST_BIN): %: %.c.o
	$(CC) $(LDLIBS) $(LDFLAGS) -o $@ $^

$(TEST_OBJ): .EXTRA_PREREQS = $(MAKEFILE)
$(TEST_OBJ): %.c.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $<

-include $(TEST_DEP)

.PHONY: all clean fclean re

all: $(NAME)

clean:
	$(RM) $(OBJ) $(DEP) $(TEST_OBJ) $(TEST_DEP)

fclean: clean
	$(RM) $(NAME) $(TEST_BIN)

re: fclean
	$(MAKE) -f $(MAKEFILE) all

.PHONY: tests

tests: $(TEST_BIN)
