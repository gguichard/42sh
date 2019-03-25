#ifndef BUILTINS_H
# define BUILTINS_H

# include "options.h"
# include "shell.h"

int		cd_builtins(t_ast *elem, t_alloc *alloc);
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
int		exec_input(t_ast *elem, t_alloc *alloc, int no_fork);

/*
************************************ TOOLS *************************************
*/

char	*get_dir(char *pwd, char **tab_path, t_opts *opts, char *buf_pwd);
char	*cd_predef(char *elem, t_list *vars, t_opts *opts, char *buf);
char	*delete_last_folder(char *dir);
void	add_new_folder(char **dir, char *folder);
char	*cd_slash(char *dir, t_opts *opts, char *buf_pwd);

int		is_builtin(t_alloc *alloc, const char *built_name);

#endif
