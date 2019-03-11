#ifndef SHELL_H
# define SHELL_H

/*
*********************************** INCLUDES ***********************************
*/

# include "builtins.h"
# include "parser_lexer.h"
# include "error.h"

/*
************************************ DEFINE ************************************
*/

# define LOGIC		5
# define OPERATOR	4
# define AGREG		2
# define REDIR		2
# define HEREDOC	1
# define CMD		0
# define NO_TYPE 	-1

/*
********************************** STRUCTURES **********************************
*/

typedef struct			s_builtin
{
	char				*name;
	int					(*builtin_fun)(t_ast *, int, char **);
}						t_builtin;

typedef	struct			s_var
{
	char				*key;
	char				*value;
	int					is_env;
	struct s_env		*next;
}						t_var;

typedef struct			s_alloc
{
	t_historic			**history;
	t_buf				*input;
	t_ast				**ast;
	t_var				**var;
	t_builtin			*builtins;
	int					fd[10];
}						t_alloc;

typedef struct			s_ast
{
	int					print;
	int					type;
	char				*heredoc;
	char				**input;
	struct s_ast		*next;
	struct s_ast		*back;
	struct s_ast		*left;
	struct s_ast		*right;
}						t_ast;

#endif
