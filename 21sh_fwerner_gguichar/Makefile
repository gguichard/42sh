# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/01/03 13:31:22 by gguichar          #+#    #+#              #
#    Updated: 2019/01/30 10:40:33 by gguichar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	21sh

SRC_DIR	=	src
SRC		=	main.c misc/signals.c vars/def_vars.c \
			vars/env.c vars/vars.c vars/shell_vars.c \
			term/setup.c term/prompt.c input/insert_mode_utils.c \
			input/input.c input/read.c input/multiline.c input/cmdline.c \
			input/esc_sequence.c input/history.c input/history_hooks.c \
			input/insert_mode.c input/insert_mode_ac.c input/visual_mode.c \
			input/cursor_hooks.c input/cursor_hooks2.c input/del_hooks.c \
			input/misc_hooks.c input/utils.c \
			lexer/tokens.c lexer/grammar.c \
			exec/binaries.c exec/execute.c exec/exec_utils.c \
			exec/single.c exec/pipeline.c exec/pipe_utils.c \
			exec/redirections.c exec/redir_output.c exec/redir_input.c \
			exec/redir_utils.c exec/heredoc.c \
			parser/cmd_inf.c parser/redirect_inf.c \
			parser/split_cmd_token.c parser/split_cmd_token_utils.c \
			parser/split_cmd_token_add.c parser/str_cmd_inf.c \
			parser/str_cmd_inf_utils.c \
			parser/token_inf.c parser/apply_escape.c \
			autocomplete/ac_check_for.c \
			autocomplete/autocomplete.c autocomplete/utils.c \
			autocomplete/ac_cmdline.c autocomplete/ac_rdir_utils.c \
			autocomplete/ac_suff_utils.c misc/hashtable_delete.c \
			misc/error.c misc/check_path.c misc/hashtable.c \
			misc/hashtable_utils.c misc/convert_path_to_tab.c misc/utils.c \
			builtins/change_dir.c builtins/exit.c builtins/echo.c \
			builtins/env.c builtins/unsetenv.c builtins/setenv.c \
			builtins/cd_utils.c \
			parser/join_token_cmd.c parser/join_token_cmd_redirect.c \
			parser/join_token_cmd_utils.c \
			vars/expand_vars.c vars/expand_vars_utils.c

OBJ_DIR	=	.obj
OBJ		=	$(SRC:.c=.o)
DEP		=	$(OBJ:.o=.d)

INC_DIR	=	includes

CC		=	gcc
CFLAGS	=	-Wall -Wextra -Werror -I libft/includes -I $(INC_DIR)

LIBFT	=	libft/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(addprefix $(OBJ_DIR)/,$(OBJ))
	$(CC) $(CFLAGS) -ltermcap -o $@ $^

$(LIBFT):
	$(MAKE) -C libft

-include $(addprefix $(OBJ_DIR)/,$(DEP))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(addprefix $(INC_DIR)/,$(INC)) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

$(OBJ_DIR):
	/bin/mkdir $@
	/bin/mkdir $@/vars
	/bin/mkdir $@/term
	/bin/mkdir $@/input
	/bin/mkdir $@/lexer
	/bin/mkdir $@/parser
	/bin/mkdir $@/exec
	/bin/mkdir $@/autocomplete
	/bin/mkdir $@/misc
	/bin/mkdir $@/builtins

clean:
	$(MAKE) -C libft clean
	/bin/rm -rf $(OBJ_DIR)

fclean: clean
	$(MAKE) -C libft fclean
	/bin/rm -f $(NAME)

re:
	$(MAKE) fclean
	$(MAKE) all

.PHONY: all clean fclean re
