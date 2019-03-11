# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/04/26 16:54:18 by tcollard          #+#    #+#              #
#    Updated: 2019/03/02 15:22:30 by jocohen          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
.PHONY: all, clean, fclean, re, debug, norme

NAME = 21sh

END = \x1b[0m
GREEN = \x1b[32m

CFLAGS = -Wall -Werror -Wextra
LDFLAGS = -Llibft
LDLIBS = -lft -ltermcap

PATH_LEXER = lexer/
PATH_PROMPT = prompt/
PATH_TOOLS = tools/
PATH_PARSER = parser/
PATH_BUILT = builtins/
PATH_OPERATOR = operator/
PATH_ERROR = error/
PATH_TERMCAPS = termcaps/

INCLUDE = ./includes
PATH_LIB = ./libft/
PATH_SRC = ./srcs/
PATH_OBJ = ./obj/

SRC =	$(PATH_LEXER)check_closing_quote.c \
		$(PATH_LEXER)ft_splitwhitespace_shell.c \
		$(PATH_LEXER)ft_strsplit_shell.c \
		$(PATH_LEXER)replace_quote.c \
		$(PATH_LEXER)replace_val_ret.c \
		$(PATH_LEXER)check_cmd_pipe.c \
		$(PATH_LEXER)lexer.c \
		$(PATH_PARSER)parser.c \
		$(PATH_PARSER)analyzer.c \
		$(PATH_PARSER)fill_ast.c \
		$(PATH_PARSER)analyzer_tools.c \
		$(PATH_BUILT)echo.c \
		$(PATH_BUILT)cd.c \
		$(PATH_BUILT)setenv.c \
		$(PATH_BUILT)unsetenv.c \
		$(PATH_BUILT)env.c \
		$(PATH_BUILT)exit.c \
		$(PATH_BUILT)exec_input.c \
		$(PATH_TOOLS)lexer_tools.c \
		$(PATH_TOOLS)clean_tools.c \
		$(PATH_TOOLS)split_tools.c \
		$(PATH_TOOLS)split_tools_2.c \
		$(PATH_TOOLS)quote_tools.c \
		$(PATH_TOOLS)parser_tools.c \
		$(PATH_TOOLS)cd_slash_tools.c \
		$(PATH_TOOLS)env_tools.c \
		$(PATH_TOOLS)env_tools_2.c \
		$(PATH_TOOLS)redirection_tools.c \
		$(PATH_TOOLS)agreg_tools.c \
		$(PATH_TOOLS)builtins_tools.c \
		$(PATH_TOOLS)back_quote_tools.c \
		$(PATH_TOOLS)heredoc_tools.c \
		$(PATH_TOOLS)ast_tools.c \
		$(PATH_TOOLS)fill_ast_tools.c \
		$(PATH_OPERATOR)pipe.c \
		$(PATH_OPERATOR)job_control.c \
		$(PATH_OPERATOR)redirection.c \
		$(PATH_OPERATOR)agregation.c \
		$(PATH_OPERATOR)heredoc.c \
		$(PATH_PROMPT)quote_prompt.c \
		$(PATH_ERROR)lexer_error.c \
		$(PATH_ERROR)parser_error.c \
		$(PATH_ERROR)error_cd.c \
		$(PATH_ERROR)exec_error.c \
		$(PATH_ERROR)error_setenv.c \
		$(PATH_ERROR)error_unsetenv.c \
		$(PATH_ERROR)error_redirection.c \
		$(PATH_ERROR)error_fd.c \
		$(PATH_ERROR)error_malloc.c \
		$(PATH_TERMCAPS)caller_display.c \
		$(PATH_TERMCAPS)cursor_fit.c \
		$(PATH_TERMCAPS)cursor_shift.c \
		$(PATH_TERMCAPS)ending_input_line.c \
		$(PATH_TERMCAPS)escape_analysis.c \
		$(PATH_TERMCAPS)execute_spe_keys.c \
		$(PATH_TERMCAPS)execute_spe_keys2.c \
		$(PATH_TERMCAPS)ft_exit.c \
		$(PATH_TERMCAPS)ft_realloc.c \
		$(PATH_TERMCAPS)ft_writestdin.c \
		$(PATH_TERMCAPS)history_movement.c \
		$(PATH_TERMCAPS)history_stuff.c \
		$(PATH_TERMCAPS)history_tree.c \
		$(PATH_TERMCAPS)input_analysis.c \
		$(PATH_TERMCAPS)line_displays.c \
		$(PATH_TERMCAPS)paste_and_cut.c \
		$(PATH_TERMCAPS)prompter.c \
		$(PATH_TERMCAPS)prompter_stuff.c \
		$(PATH_TERMCAPS)selection_buffer.c \
		$(PATH_TERMCAPS)set_term.c \
		$(PATH_TERMCAPS)shell.c \
		$(PATH_TERMCAPS)signal_control.c \
		$(PATH_TERMCAPS)return_status.c

OBJ = $(SRC:.c=.o)

SRCS = $(addprefix $(PATH_SRC),$(SRC))
OBJS = $(addprefix $(PATH_OBJ),$(OBJ))

all: $(NAME)

$(NAME): $(OBJS)
	@make -C $(PATH_LIB)
	@gcc $(CFLAGS) $(LDFLAGS) $(LDLIBS) -o $@ $(OBJS)
	@echo "\n$(NAME):\t\t\t$(GREEN)[READY]\n\t\t¯\_(ツ)_/¯$(END)"

$(PATH_OBJ)%.o : $(PATH_SRC)%.c $(INCLUDE)/shell.h Makefile
	@mkdir $(PATH_OBJ) 2> /dev/null || true
	@mkdir $(PATH_OBJ)$(PATH_LEXER) 2> /dev/null || true
	@mkdir $(PATH_OBJ)$(PATH_TOOLS) 2> /dev/null || true
	@mkdir $(PATH_OBJ)$(PATH_PARSER) 2> /dev/null || true
	@mkdir $(PATH_OBJ)$(PATH_PROMPT) 2> /dev/null || true
	@mkdir $(PATH_OBJ)$(PATH_BUILT) 2> /dev/null || true
	@mkdir $(PATH_OBJ)$(PATH_ERROR) 2> /dev/null || true
	@mkdir $(PATH_OBJ)$(PATH_OPERATOR) 2> /dev/null || true
	@mkdir $(PATH_OBJ)$(PATH_TERMCAPS) 2> /dev/null || true
	@gcc $(CFLAGS) -g3 -I $(INCLUDE) -o $@ -c $<
	@echo " "
	@printf "\033[1A"
	@./progress_bar.sh $(NAME) $(shell ls -lR $(PATH_SRC) 2> /dev/null | grep -c -o "\.c") $(shell ls -lR $(PATH_OBJ) 2> /dev/null | grep -c -o "\.o")

clean:
	@/bin/rm -f $(OBJS)
	@make clean -C $(PATH_LIB)
	@rmdir $(PATH_OBJ)$(PATH_LEXER) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_TOOLS) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_PARSER) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_BUILT) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_PROMPT) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_ERROR) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_OPERATOR) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_TERMCAPS) 2> /dev/null || true
	@rmdir $(PATH_OBJ) 2> /dev/null || true

fclean: clean
	@make fclean -C $(PATH_LIB)
	@rm -f $(NAME)
	@echo "$(NAME):\t\t\t$(GREEN)[CLEAN]$(END)"

re: fclean all

norme:
	norminette $(SRC)
	norminette ./includes/

debug: $(OBJS)
	@make -C $(PATH_LIB)
	@gcc $(CFLAGS) $(LDFLAGS) $(LDLIBS) -fsanitize=address -g3 -o $(NAME) $(OBJS)
