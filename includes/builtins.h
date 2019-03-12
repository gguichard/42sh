#ifndef BUILTINS_H
# define BUILTINS_H

# include "shell.h"

int		cd_builtins(t_ast *elem, t_var **lst_env, t_alloc **alloc);
int		echo_builtins(t_ast *elem, t_var **lst_env, t_alloc **alloc);
int		env_builtins(t_ast *elem, t_var **lst_env, t_alloc **alloc);
int		env_cp(char **env, t_var **lst_env);
int		exit_builtins(t_ast *elem, t_var **lst_env, t_alloc **alloc);
int		setenv_builtins(t_ast *elem, t_var **lst_env, t_alloc **alloc);
int		unsetenv_builtins(t_ast *elem, t_var **lst_env, t_alloc **alloc);
int		hash_builtins(t_ast *elem, t_var **lst_env, t_alloc **alloc);

/*
************************************ TOOLS *************************************
*/

t_var	*find_elem_env(t_var **lst, char *key);
t_var	*get_last_elem_env(t_var **lst);
void	add_elem_env(t_var **lst, char *key, char *value);
void	add_shlvl(t_var **lst);
void	display_env(t_var *lst);
void	convert_lst_tab(t_var *lst_env, char ***tab_str);
char	*get_env_value(t_var *lst_env, char *str);

int		is_builtins(t_alloc *alloc, const char *built_name);


#endif
