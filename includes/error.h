#ifndef ERROR_H
# define ERROR_H

# include "shell.h"

int		ft_error_parse_redir(char **input);
int		check_error_lst(t_ast *lst);
int		ft_error_splitshell(void);
int		ft_error_redir_format(char *ope, int len);
void	ft_exit_malloc(void);
void	error_redir(char *file);
void	error_access(char *file);
int		check_arg_cd(t_ast *elem, int i);
int		error_cd(char *err, int type);
int		exec_error(int err, char *files);
int		exec_right_error(int err, char *files, char ***path_all);
int		error_setenv(int i);
int		error_unsetenv(int i, char *s);
int		ft_fd_exist(char *str_fd);

#endif
