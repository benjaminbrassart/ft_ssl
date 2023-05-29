# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/23 16:27:42 by bbrassar          #+#    #+#              #
#    Updated: 2023/05/29 10:02:31 by bbrassar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := ft_ssl
DIR_LIBFT := libft
NAME_LIBFT := $(DIR_LIBFT)/libft.a
NAME_LIBFT_SSL := libft_ssl.a

CC := cc
CFLAGS := -Wall -Werror -Wextra -c -MMD -MP -I. -g3 -Iinclude -I$(DIR_LIBFT)/include

AR := ar
ARFLAGS := rs

LDLIBS := -lft_ssl -lft -lreadline
LDFLAGS := -L. -L$(dir $(NAME_LIBFT))

RM := rm -vf
MKDIR := mkdir -vp

DIR_SRC := src
DIR_OBJ := obj

SRC += byteswap.c
SRC += rotate.c
SRC += digest/md5.c
SRC += digest/sha224.c
SRC += digest/sha256.c
SRC += digest/sha384.c
SRC += digest/sha512.c

OBJ := $(SRC:%.c=$(DIR_OBJ)/%.o)
DEP := $(OBJ:.o=.d)

SRC_MAIN := main.c interactive.c command.c hex.c hash.c
OBJ_MAIN := $(SRC_MAIN:%.c=$(DIR_OBJ)/%.o)
DEP_MAIN := $(OBJ_MAIN:.o=.d)

$(NAME): $(OBJ_MAIN) $(NAME_LIBFT) $(NAME_LIBFT_SSL)
	$(CC) $(filter %.o,$^) -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJ) $(OBJ_MAIN): $(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@$(MKDIR) $(@D)
	$(CC) $(CFLAGS) $< -o $@

$(NAME_LIBFT_SSL): $(OBJ)
	$(AR) $(ARFLAGS) $@ $?

$(NAME_LIBFT):
	@$(MAKE) -C $(@D)

-include $(DEP) $(DEP_MAIN)

.PHONY: all clean fclean re unit-test

all: $(NAME)

clean:
	@$(RM) -r $(DIR_OBJ)
	@$(MAKE) -C $(DIR_LIBFT) clean

fclean: clean
	@$(RM) $(NAME) $(NAME_LIBFT_SSL)
	@$(MAKE) -C $(DIR_LIBFT) fclean

re: fclean all

unit-test:
	@cd tests/unit && sh test.sh
