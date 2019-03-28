# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/07/29 17:35:18 by gguichar          #+#    #+#              #
#    Updated: 2019/03/28 11:08:44 by gguichar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	libft.a

SRC		=				\
ft_abs.c				\
ft_atoi.c				\
ft_atoi_base.c			\
ft_bzero.c				\
ft_ceil.c				\
ft_isalnum.c			\
ft_isalpha.c			\
ft_isascii.c			\
ft_isdigit.c			\
ft_islower.c			\
ft_isprint.c			\
ft_isspace.c			\
ft_isupper.c			\
ft_itoa.c				\
ft_itoa_base.c			\
ft_join.c				\
ft_levenshtein.c		\
ft_llsize.c				\
ft_lltoa.c				\
ft_lltoa_base.c			\
ft_lstadd.c				\
ft_lstdel.c				\
ft_lstdelone.c			\
ft_lstempty.c			\
ft_lstfree.c			\
ft_lstiter.c			\
ft_lstmap.c				\
ft_lstnew.c				\
ft_lstpeek.c			\
ft_lstpop.c				\
ft_lstpush.c			\
ft_lstsize.c			\
ft_max.c				\
ft_memalloc.c			\
ft_memccpy.c			\
ft_memchr.c				\
ft_memcmp.c				\
ft_memcpy.c				\
ft_memdel.c				\
ft_memmove.c			\
ft_memset.c				\
ft_min.c				\
ft_pow.c				\
ft_putchar.c			\
ft_putchar_fd.c			\
ft_putendl.c			\
ft_putendl_fd.c			\
ft_putnbr.c				\
ft_putnbr_fd.c			\
ft_putstr.c				\
ft_putstr_fd.c			\
ft_putwchar.c			\
ft_simple_atoi.c		\
ft_strcat.c				\
ft_strchr.c				\
ft_strclr.c				\
ft_strcmp.c				\
ft_strcpy.c				\
ft_strdel.c				\
ft_strdup.c				\
ft_strequ.c				\
ft_striter.c			\
ft_striteri.c			\
ft_strjoin.c			\
ft_strjoin_free.c		\
ft_strlcat.c			\
ft_strlen.c				\
ft_strmap.c				\
ft_strmapi.c			\
ft_strncat.c			\
ft_strncmp.c			\
ft_strncpy.c			\
ft_strndup.c			\
ft_strnequ.c			\
ft_strnew.c				\
ft_strnlen.c			\
ft_strnstr.c			\
ft_strrchr.c			\
ft_strreplace.c			\
ft_strreplace_free.c	\
ft_strreplace_inside.c	\
ft_strsplit.c			\
ft_strstr.c				\
ft_strstrim.c			\
ft_strsub.c				\
ft_strtab_count.c		\
ft_strtab_free.c		\
ft_strtol.c				\
ft_strtolower.c			\
ft_strtoupper.c			\
ft_strtrim.c			\
ft_tolower.c			\
ft_toupper.c			\
ft_ulltoa.c				\
ft_ulltoa_base.c		\
ft_wcharlen.c			\
ft_wstrchr.c			\
ft_wstrlen.c			\
get_next_line.c
SRC_DIR	=	src

OBJ		=	$(SRC:.c=.o)
OBJ_DIR	=	.obj

PF_DIR	=	printf
PF_LIB	=	$(PF_DIR)/printf.a

OPT_DIR	=	options
OPT_LIB	=	$(OPT_DIR)/options.a

CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -I includes

all: $(NAME)

$(NAME): $(PF_LIB) $(OPT_LIB) $(addprefix $(OBJ_DIR)/,$(OBJ))
	libtool -static -o $(NAME) $^

$(PF_LIB):
	$(MAKE) -C $(PF_DIR)

$(OPT_LIB):
	$(MAKE) -C $(OPT_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJ_DIR):
	/bin/mkdir $@

clean:
	$(MAKE) -C $(PF_DIR) clean
	$(MAKE) -C $(OPT_DIR) clean
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C $(PF_DIR) fclean
	$(MAKE) -C $(OPT_DIR) fclean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
