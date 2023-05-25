# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/23 16:27:42 by bbrassar          #+#    #+#              #
#    Updated: 2023/05/25 03:19:15 by bbrassar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := ft_ssl
DIR_LIBFT := libft
NAME_LIBFT := $(DIR_LIBFT)/libft.a

CC := cc
CFLAGS := -Wall -Werror -Wextra -c -MMD -MP -I. -g3 -Iinclude -I.

LDLIBS := -lft
LDFLAGS := -L$(dir $(NAME_LIBFT))

RM := rm -vf
MKDIR := mkdir -vp

DIR_SRC := src
DIR_OBJ := obj

SRC := main.c
SRC += md5.c
SRC += sha22.c
SRC += hash.c
SRC += hex.c
OBJ := $(SRC:%.c=$(DIR_OBJ)/%.o)
DEP := $(OBJ:.o=.d)

$(NAME): $(OBJ) $(NAME_LIBFT)
	$(CC) $(filter %.o,$^) -o $@ $(LDFLAGS) $(LDLIBS)

$(OBJ): $(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@$(MKDIR) $(@D)
	$(CC) $(CFLAGS) $< -o $@

$(NAME_LIBFT):
	@$(MAKE) $(MAKEFLAGS) -C $(@D)

-include $(DEP)

.PHONY: all clean fclean re

all: $(NAME)

clean:
	@$(RM) -r $(DIR_OBJ)
	@$(MAKE) -C $(DIR_LIBFT) clean

fclean: clean
	@$(RM) $(NAME)
	@$(MAKE) -C $(DIR_LIBFT) fclean

re: fclean all
