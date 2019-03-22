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

/*
************************************ DEFINE ************************************
*/

# define LOGIC		5
# define OPERATOR	4
# define ASSIGN		3
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

struct					s_alloc;
typedef int				(*t_built_fun)(t_ast *, struct s_alloc *);

typedef struct			s_builtin
{
	const char			*name;
	t_built_fun			built_fun;
}						t_builtin;

typedef struct			s_alloc
{
	t_historic			*history;
	t_buf				*input;
	t_ast				*ast;
	t_var				**var;
	t_builtin			*builtins;
	t_hashtable			*exectable;
	t_hashtable			*aliastable;
	int					fd[10];
}						t_alloc;

typedef int				(*t_dispatch)(t_ast *elem, t_alloc *alloc, int no_fork);

/*
************************************ TOOLS *************************************
*/

void	delete_str_tab(char **tab_str);
void	del_lst_env(t_var **lst);
void	del_lst_ast(t_ast **lst);
void	del_double_tab(char **tab1, char **tab2);
void	del_alloc(t_alloc *alloc);
void	set_alloc(t_alloc *al, t_var **lst);
int		ret_status(void);
int		replace_val_ret(char **str, int i, int x);
void	insert_new_elem(t_var **lst, t_var *new);

//TOOLS TO PRINT LST AST
void	read_lst(t_ast *lst, int active);
void	read_sort_descent(t_ast *sort, int active);
void	reinit_print(t_ast *lst, int active);

// CLEN AST
void	del_ast(t_ast **lst);
void	del_elem_ast(t_ast **lst);

/*
*********************************** GLOBALS ***********************************
*/

int						g_in_exec;
int						g_pid;
int						g_ret[2];
int						g_resize;
char					*g_clip;

#endif
