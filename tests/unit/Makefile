# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bbrassar <bbrassar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/25 22:20:26 by bbrassar          #+#    #+#              #
#    Updated: 2023/05/25 22:34:34 by bbrassar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

DIR_BASE := ../..
DIR_LIBFT := $(DIR_BASE)/libft
NAME_LIBFT := $(DIR_LIBFT)/libft.a
NAME_LIBFT_SSL := $(DIR_BASE)/libft_ssl.a

CXX := c++
CXXFLAGS := -Wall -Werror -Wextra -I$(DIR_BASE)/include

LDLIBS := -l ft -l ft_ssl
LDFLAGS := -L $(DIR_LIBFT) -L $(DIR_BASE)

SRC := test_rotate.cpp
BIN := $(SRC:.cpp=.out)

all: $(BIN)

$(BIN): %.out: %.cpp $(NAME_LIBFT_SSL) $(NAME_LIBFT)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS) $(LDLIBS)

$(NAME_LIBFT) $(NAME_LIBFT_SSL):
	$(MAKE) $(MAKEFLAGS) -C $(@D) $(@F)

clean:

fclean:

re: fclean all
