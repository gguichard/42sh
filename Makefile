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
PATH_TOOLS = tools/

INCLUDE = ./includes
PATH_LIB = ./libft/
PATH_SRC = ./srcs/
PATH_OBJ = ./obj/

SRC =	main.c \
		$(PATH_ERROR)error_malloc.c \
		$(PATH_ERROR)lexer_error.c \
		$(PATH_ERROR)parser_error.c \
		$(PATH_LEXER)check_closing_quote.c \
		$(PATH_LEXER)check_cmd_pipe.c \
		$(PATH_LEXER)ft_splitwhitespace_shell.c \
		$(PATH_LEXER)ft_strsplit_shell.c \
		$(PATH_LEXER)lexer.c \
		$(PATH_LEXER)replace_quote.c \
		$(PATH_LEXER)replace_val_ret.c \
		$(PATH_PARSER)fill_ast.c \
		$(PATH_PARSER)parser.c \
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
		$(PATH_TOOLS)builtins_tools.c

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
	@mkdir $(PATH_OBJ)$(PATH_TOOLS) 2> /dev/null || true
	@gcc $(CFLAGS) -g3 -I $(INCLUDE) -o $@ -c $<

clean:
	@/bin/rm -f $(OBJS)
	@make clean -C $(PATH_LIB)
	@rmdir $(PATH_OBJ)$(PATH_BUILT) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_ERROR) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_EXEC) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_LEXER) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_PARSER) 2> /dev/null || true
	@rmdir $(PATH_OBJ)$(PATH_TOOLS) 2> /dev/null || true
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
