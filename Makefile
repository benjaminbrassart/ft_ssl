# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/23 16:27:42 by bbrassar          #+#    #+#              #
#    Updated: 2023/05/23 18:56:52 by bbrassar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := ft_ssl

CC := cc
CFLAGS := -Wall -Werror -Wextra -c -MMD -MP -I. -g3

DIR_SRC := .
DIR_OBJ := obj
DIR_DOCS := docs

SRC := md5.c
# SRC += sha2.c
OBJ := $(SRC:%.c=$(DIR_OBJ)/%.o)
DEP := $(OBJ:.o=.d)

$(NAME): $(OBJ)
	$(CC) $^ -o $@

$(OBJ): $(DIR_OBJ)/%.o: $(DIR_SRC)/%.c
	@mkdir -vp $(@D)
	$(CC) $(CFLAGS) $< -o $@

-include $(DEP)

.PHONY: all clean fclean re docs

all: $(NAME)

clean:
	@rm -vfr $(DIR_OBJ) $(DIR_DOCS)

fclean: clean
	rm -vf $(NAME)

re: fclean all

docs: Doxyfile main.dox $(SRC:%:$(DIR_SRC)/%)
	@doxygen Doxyfile
