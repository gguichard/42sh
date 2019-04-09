/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 16:26:52 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/09 17:35:07 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# define SIGNALS_ON		1
/*
*********************************** INCLUDES ***********************************
*/

//for atom completion
# include "../libft/includes/libft.h"
# include "../libft/includes/get_next_line.h"
# include "../libft/includes/printf.h"

# include "libft.h"
# include "cmdline.h"
# include "hashtable.h"

/*
************************************ DEFINE ************************************
*/

# define AST_CMD_SEP	6
# define AST_JOB		5
# define AST_LOGIC		4
# define AST_PIPE		3
# define AST_ASSIGN		2
# define AST_CMD		1
# define AST_REDIR		0
# define AST_NO_TYPE	-1

/*
********************************** STRUCTURES **********************************
*/

typedef struct s_alloc	t_alloc;

typedef struct			s_ast
{
	int					type;
	int					fd[2];
	char				**input;
	struct s_ast		*back;
	struct s_ast		*left;
	struct s_ast		*right;
}						t_ast;

typedef struct			s_builtin
{
	const char			*name;
	int					(*built_fun)(t_ast *, t_alloc *);
}						t_builtin;

struct					s_alloc
{
	int					argc;
	char				**argv;
	int					is_interactive;
	int					exit_rdy;
	int					ret_val;
	pid_t				last_bg;
	pid_t				ppid;
	t_cmdline			cmdline;
	char				*full_input;
	t_ast				*ast;
	t_list				*vars;
	const t_builtin		*builtins;
	t_hashtable			*exectable;
	t_hashtable			*aliastable;
};

/*
************************************ TOOLS *************************************
*/

const char				*get_home_directory(t_list *vars);

int						setup_alloc(t_alloc *alloc, int argc, char **argv
		, char **environ);

void					del_alloc(t_alloc *alloc);
void					del_ast(t_ast **lst);
void					*del_elem_ast(t_ast **lst);

char					*get_tempfile_path(t_alloc *alloc
		, const char *file_prefix);

/*
********************************** GLOBALES ***********************************
*/

t_list					*g_jobs;
int						g_sig;

#endif
