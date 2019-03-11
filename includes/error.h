#ifndef ERROR_H
# define ERROR_H

# include "shell.h"

int		ft_error_parse_redir(char **input);
int		check_error_lst(t_ast *lst);
int		ft_error_splitshell(void);
int		ft_error_redir_format(char *ope, int len);
void	ft_exit_malloc(void);

#endif
