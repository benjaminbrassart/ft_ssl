# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/23 16:27:42 by bbrassar          #+#    #+#              #
#    Updated: 2023/05/24 03:43:21 by bbrassar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := ft_ssl

CC := cc
CFLAGS := -Wall -Werror -Wextra -c -MMD -MP -I. -g3 -Iinclude

DIR_SRC := src
DIR_OBJ := obj

SRC := main.c
SRC += md5.c
# SRC += sha2.c
SRC += hash.c
SRC += hex.c
OBJ := $(SRC:%.c=$(DIR_OBJ)/%.o)
DEP := $(OBJ:.o=.d)

$(NAME): $(OBJ)
	$(CC) $^ -o $@

$(OBJ): $(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) $< -o $@

-include $(DEP)

.PHONY: all clean fclean re

all: $(NAME)

clean:
	@rm -vfr $(DIR_OBJ)

fclean: clean
	rm -vf $(NAME)

re: fclean all
