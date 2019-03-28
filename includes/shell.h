#ifndef SHELL_H
# define SHELL_H

/*
*********************************** INCLUDES ***********************************
*/

//for atom completion
# include "../libft/includes/get_next_line.h"
# include "../libft/includes/libft.h"
# include "../libft/includes/printf.h"

# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <fcntl.h>
# include <signal.h>
# include <stdbool.h>
# include "libft.h"
# include "get_next_line.h"
# include "cmdline.h"
# include "hashtable.h"

/*
************************************ DEFINE ************************************
*/

# define CMD_SEP	5
# define LOGIC		4
# define OPERATOR	3
# define ASSIGN		2
# define CMD		1
# define REDIR		0
# define NO_TYPE	-1

/*
********************************** STRUCTURES **********************************
*/

typedef struct			s_exec_opt
{
	bool				fork;
	bool				wait_hang;
}						t_exec_opt;

typedef struct			s_ast
{
	int					print;
	int					type;
	int					fd[2];
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
	pid_t				last_bg;
	pid_t				pid;
	t_cmdline			cmdline;
	t_ast				*ast;
	t_list				*vars;
	const t_builtin		*builtins;
	t_hashtable			*exectable;
	t_hashtable			*aliastable;
	int					fd[10];
}						t_alloc;

typedef int				(*t_dispatch)(t_ast *elem, t_alloc *alloc, t_exec_opt *opt);

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

t_list					*g_jobs;

#endif
