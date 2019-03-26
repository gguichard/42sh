#ifndef SHELL_H
# define SHELL_H

/*
*********************************** INCLUDES ***********************************
*/

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <fcntl.h>
# include "libft.h"
# include "get_next_line.h"
# include "cmdline.h"
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
# define NO_TYPE	-1

/*
********************************** STRUCTURES **********************************
*/

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
	int					argc;
	char				**argv;
	int					ret_val;
	t_cmdline			cmdline;
	t_ast				*ast;
	t_list				*vars;
	const t_builtin		*builtins;
	t_hashtable			*exectable;
	t_hashtable			*aliastable;
	int					fd[10];
}						t_alloc;

typedef int				(*t_dispatch)(t_ast *elem, t_alloc *alloc, int no_fork);

/*
*********************************** CMDLINE ************************************
*/

int		init_cmdline(t_alloc *alloc, t_cmdline *cmdline);
char	*read_cmdline(t_alloc *alloc, t_cmdline *cmdline);
char	*get_history_file_path(t_alloc *alloc);
int		load_history_file_entries(t_alloc *alloc, t_history *history);
int		save_history_entries(t_alloc *alloc, t_history *history);

/*
************************************ TOOLS *************************************
*/

int		setup_alloc(t_alloc *alloc, int argc, char **argv, char **environ);

void	del_lst_ast(t_ast **lst);
void	del_alloc(t_alloc *alloc);
int		ret_status(void);
int		replace_val_ret(char **str, int i, int x);

//TOOLS TO PRINT LST AST
void	read_lst(t_ast *lst, int active);
void	read_sort_descent(t_ast *sort, int active);
void	reinit_print(t_ast *lst, int active);

// CLEN AST
void	del_ast(t_ast **lst);
void	del_elem_ast(t_ast **lst);
void	delete_str_tab(char **tab_str);


/*
*********************************** GLOBALS ***********************************
*/

int						g_in_exec;
int						g_pid;
int						g_ret[2];

#endif
