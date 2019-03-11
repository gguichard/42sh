#ifndef BUILTINS_H
# define BUILTINS_H

int	cd_builtins(t_ast *elem, t_var **lst_env, t_alloc **alloc);
int	echo_builtins(t_ast *elem, t_var **lst_env, t_alloc **alloc);
int	env_builtins(t_ast *elem, t_var **lst_env, t_alloc **alloc);
int	env_cp(char **env, t_var **lst_env);
int	exit_builtins(t_ast *elem, t_var **lst_env, t_alloc **alloc);
int	setenv_builtins(t_ast *elem, t_var **lst_env, t_alloc **alloc);
int	unsetenv_builtins(t_ast *elem, t_var **lst_env, t_alloc **alloc);

#endif
