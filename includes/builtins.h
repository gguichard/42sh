#ifndef BUILTINS_H
# define BUILTINS_H

# include <unistd.h>
# include "shell.h"

int		builtin_cd(t_ast *elem, t_alloc *alloc);
int		builtin_echo(t_ast *elem, t_alloc *alloc);
int		builtin_exit(t_ast *elem, t_alloc *alloc);
int		builtin_hash(t_ast *elem, t_alloc *alloc);
int		builtin_alias(t_ast *elem, t_alloc *alloc);
int		builtin_unalias(t_ast *elem, t_alloc *alloc);
int		builtin_type(t_ast *elem, t_alloc *alloc);
int		builtin_set(t_ast *elem, t_alloc *alloc);
int		builtin_unset(t_ast *elem, t_alloc *alloc);
int		builtin_export(t_ast *elem, t_alloc *alloc);
int		builtin_jobs(t_ast *elem, t_alloc *alloc);
int		builtin_fg(t_ast *elem, t_alloc *alloc);
int		builtin_bg(t_ast *elem, t_alloc *alloc);
int		builtin_test(t_ast *elem, t_alloc *alloc);
int		builtin_exec(t_ast *elem, t_alloc *alloc);
int		builtin_source(t_ast *elem, t_alloc *alloc);

/*
************************************ TOOLS *************************************
*/

void	set_dir_to_canonical_form(char *dir);
int		is_builtin(t_alloc *alloc, const char *built_name);
void	check_exit_cmd(t_ast *elem);

#endif
