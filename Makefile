.PHONY: all, clean, fclean, re, debug, norme

NAME = 42sh

END = \x1b[0m
GREEN = \x1b[32m

CFLAGS = -Wall -Werror -Wextra
LDFLAGS = -Llibft
LDLIBS = -lft -ltermcap

PATH_BUILT = builtins/
PATH_ERROR = error/
PATH_EXEC = exec/
PATH_LEXER = lexer/
PATH_PARSER = parser/
PATH_OPERATOR = operator/
PATH_TOOLS = tools/
PATH_HASHTABLE = hashtable/
PATH_PATH = path/

INCLUDE = ./includes
INCLUDE_LIBFT = ./libft/includes
PATH_LIB = ./libft/
PATH_SRC = ./srcs/
PATH_OBJ = ./obj/

SRC =	main.c \
		$(PATH_ERROR)error_malloc.c \
		$(PATH_ERROR)lexer_error.c \
		$(PATH_ERROR)parser_error.c \
		$(PATH_ERROR)error_redirection.c \
		$(PATH_ERROR)error_cd.c \
		$(PATH_ERROR)error_fd.c \
		$(PATH_ERROR)error_setenv.c \
		$(PATH_ERROR)error_unsetenv.c \
		$(PATH_ERROR)exec_error.c \
		$(PATH_ERROR)error_utils.c \
		$(PATH_LEXER)check_closing_quote.c \
		$(PATH_LEXER)check_cmd_pipe.c \
		$(PATH_LEXER)ft_splitwhitespace_shell.c \
		$(PATH_LEXER)ft_strsplit_shell.c \
		$(PATH_LEXER)lexer.c \
		$(PATH_LEXER)replace_quote.c \
		$(PATH_LEXER)replace_val_ret.c \
		$(PATH_PARSER)fill_ast.c \
		$(PATH_PARSER)parser.c \
		$(PATH_PARSER)analyzer.c \
		$(PATH_PARSER)str_cmd_inf.c \
		$(PATH_PARSER)str_cmd_inf_utils.c \
		$(PATH_PARSER)str_cmd_inf_escape.c \
		$(PATH_OPERATOR)agregation.c \
		$(PATH_OPERATOR)heredoc.c \
		$(PATH_OPERATOR)redirection.c \
		$(PATH_OPERATOR)job_control.c \
		$(PATH_OPERATOR)pipe.c \
		$(PATH_BUILT)cd.c \
		$(PATH_BUILT)echo.c \
		$(PATH_BUILT)exec_input.c \
		$(PATH_BUILT)exit.c \
		$(PATH_BUILT)hash.c \
		$(PATH_TOOLS)clean_tools.c \
		$(PATH_TOOLS)printer_ast.c \
		$(PATH_TOOLS)back_quote_tools.c \
		$(PATH_TOOLS)lexer_tools.c \
		$(PATH_TOOLS)split_tools.c \
		$(PATH_TOOLS)split_tools_2.c \
		$(PATH_TOOLS)fill_ast_tools.c \
		$(PATH_TOOLS)ast_tools.c \
		$(PATH_TOOLS)parser_tools.c \
		$(PATH_TOOLS)env_tools.c \
		$(PATH_TOOLS)env_tools_2.c \
		$(PATH_TOOLS)builtins_tools.c \
		$(PATH_TOOLS)alloc_starter_tools.c \
		$(PATH_BUILT)bg.c\
		$(PATH_BUILT)export.c\
		$(PATH_BUILT)fg.c\
		$(PATH_BUILT)job.c\
		$(PATH_BUILT)set.c\
		$(PATH_BUILT)type.c\
		$(PATH_BUILT)unset.c\
		$(PATH_TOOLS)analyzer_tools.c \
		$(PATH_TOOLS)agreg_tools.c \
		$(PATH_TOOLS)heredoc_tools.c \
		$(PATH_TOOLS)redirection_tools.c \
		$(PATH_TOOLS)cd_slash_tools.c \
		$(PATH_TOOLS)cd_tools.c \
		$(PATH_HASHTABLE)exectable.c \
		$(PATH_HASHTABLE)aliastable.c \
		$(PATH_HASHTABLE)hashtable.c \
		$(PATH_HASHTABLE)hashtable_delete.c \
		$(PATH_HASHTABLE)hashtable_utils.c \
		$(PATH_PATH)check_path.c \
		$(PATH_PATH)convert_path_to_tab.c \
		$(PATH_PATH)search_exec.c \

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
	@mkdir $(PATH_OBJ)$(PATH_BUILT) 2> /dev/null || true
	@mkdir $(PATH_OBJ)$(PATH_ERROR) 2> /dev/null || true
	@mkdir $(PATH_OBJ)$(PATH_EXEC) 2> /dev/null || true
	@mkdir $(PATH_OBJ)$(PATH_LEXER) 2> /dev/null || true
	@mkdir $(PATH_OBJ)$(PATH_PARSER) 2> /dev/null || true
	@mkdir $(PATH_OBJ)$(PATH_OPERATOR) 2> /dev/null || true
	@mkdir $(PATH_OBJ)$(PATH_TOOLS) 2> /dev/null || true
	@mkdir $(PATH_OBJ)$(PATH_HASHTABLE) 2> /dev/null || true
	@mkdir $(PATH_OBJ)$(PATH_PATH) 2> /dev/null || true
	@gcc $(CFLAGS) -g3 -I $(INCLUDE) -I $(INCLUDE_LIBFT) -o $@ -c $<

clean:
	@/bin/rm -f $(OBJS)
	@make clean -C $(PATH_LIB)
	@rmdir $(PATH_OBJ)$(PATH_BUILT) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_ERROR) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_EXEC) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_LEXER) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_PARSER) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_OPERATOR) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_TOOLS) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_HASHTABLE) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_PATH) 2> /dev/null || true
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
