#ifndef SHELL_H
# define SHELL_H

/*
*********************************** INCLUDES ***********************************
*/

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <fcntl.h>
# include "../libft/includes/get_next_line.h"
# include "../libft/includes/libft.h"
# include "../libft/includes/printf.h"
# include "hashtable.h"
// # include "error.h"

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


typedef struct			s_cursor
{
	size_t				l;
	size_t				c;
}						t_cursor;

typedef struct			s_buf
{
	char				*s;
	size_t				x;
	t_cursor			pos;
	size_t				buf_size;
}						t_buf;

typedef struct			s_historic
{
	char				*origin;
	char				*modif;
	struct s_historic	*next;
	struct s_historic	*prev;
}						t_historic;


typedef	struct			s_var
{
	char				*key;
	char				*value;
	int					is_env;
	struct s_var		*next;
}						t_var;

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

typedef struct			s_builtin
{
	char				*name;
	int					(*builtin_fun)(t_ast *, int, char **);
}						t_builtin;

typedef struct			s_alloc
{
	t_historic			**history;
	t_buf				*input;
	t_ast				**ast;
	t_var				**var;
	t_builtin			*builtins;
	t_hashtable			*exectable;
	int					fd[10];
}						t_alloc;

typedef int				(*t_dispatch)(t_ast*, t_var **lst_env, char **tab_path,
						t_alloc **alloc);
typedef int				(*t_builtins)(t_ast *elem, t_var **lst_env,
						t_alloc **alloc);

/*
************************************ TOOLS *************************************
*/

void	delete_str_tab(char **tab_str);
void	del_lst_env(t_var **lst);
void	del_lst_ast(t_ast **lst);
void	del_double_tab(char **tab1, char **tab2);
void	del_alloc(t_alloc **alloc);
int		ret_status(void);
int		replace_val_ret(char **str, int i, int x);


/*
*********************************** GLOBALS ***********************************
*/

int						g_in_exec;
int						g_pid;
int						g_ret[2];
int						g_resize;
char					*g_clip;

// # include "parser_lexer.h"
// # include "autocomplete.h"
// # include "builtins.h"
// # include "convert_path_to_tab.h"
// # include "error.h"
// # include "expand_vars.h"

#endif
