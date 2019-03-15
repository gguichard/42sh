#ifndef BUILTINS_H
# define BUILTINS_H

# include "shell.h"

int		cd_builtins(t_ast *elem, t_alloc *alloc);
int		echo_builtins(t_ast *elem, t_alloc *alloc);
int		env_builtins(t_ast *elem, t_alloc *alloc);
int		env_cp(char **env, t_var *lst_env);
int		exit_builtins(t_ast *elem, t_alloc *alloc);
int		setenv_builtins(t_ast *elem, t_alloc *alloc);
int		unsetenv_builtins(t_ast *elem, t_alloc *alloc);
int		hash_builtins(t_ast *elem, t_alloc *alloc);
int		alias_builtins(t_ast *elem, t_alloc *alloc);
int		type_builtins(t_ast *elem, t_alloc *alloc);
int		set_builtins(t_ast *elem, t_alloc *alloc);
int		unset_builtins(t_ast *elem, t_alloc *alloc);
int		export_builtins(t_ast *elem, t_alloc *alloc);
int		job_builtins(t_ast *elem, t_alloc *alloc);
int		fg_builtins(t_ast *elem, t_alloc *alloc);
int		bg_builtins(t_ast *elem, t_alloc *alloc);
int		exec_input(t_ast *elem, t_alloc *alloc, int no_fork);

/*
************************************ TOOLS *************************************
*/

t_var	*find_elem_env(t_var *lst, char *key);
t_var	*get_last_elem_env(t_var *lst);
void	add_elem_env(t_var *lst, char *key, char *value);
void	add_shlvl(t_var *lst);
void	display_env(t_var *lst);
void	convert_lst_tab(t_var *lst_env, char ***tab_str);
char	*get_env_value(t_var *lst_env, char *str);
char	*get_dir(char *pwd, char **tab_path, int options,
		char *buf_pwd);
char	*cd_predef(char *elem, t_var *lst_env,
		int options, char *buf);
char	*delete_last_folder(char *dir);
void	add_new_folder(char **dir, char *folder);
char	*cd_slash(char *dir, int options, char *buf_pwd);


int		is_builtins(t_alloc *alloc, const char *built_name);


#endif
