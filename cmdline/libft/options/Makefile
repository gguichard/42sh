# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/12 09:21:09 by gguichar          #+#    #+#              #
#    Updated: 2018/12/20 21:14:39 by gguichar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= 	options.a

SRC		= 	parsing.c utils.c
SRC_DIR	= 	src

OBJ		= 	$(SRC:.c=.o)
OBJ_DIR	= 	.obj

CC		= 	gcc
CFLAGS	= 	-Wall -Wextra -Werror -O2 -I ../includes

all: $(NAME)

$(NAME): $(addprefix $(OBJ_DIR)/,$(OBJ))
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c includes/options.h  | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	/bin/mkdir $@

clean:
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
