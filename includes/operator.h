#ifndef OPEARTOR_H
# define OPEARTOR_H

# include "shell.h"
# include "error.h"

/*
********************************** OPERATOR ************************************
*/

int						do_pipe(t_ast *elem, t_alloc *alloc);
int						do_not_fork(t_ast *elem, t_alloc *alloc);
int						job_control(t_ast *elem, t_alloc *alloc);
int						redirection(t_ast *elem, t_alloc *alloc, int no_fork);
int						heredoc(t_ast *elem, t_alloc *alloc);
int						complete_heredoc(t_ast *lst, t_alloc *alloc);

/*
******************************** REDIRECTION ***********************************
*/

int						redirection_1(t_ast *elem, t_alloc *alloc, int no_fork);
int						redirection_2(t_ast *elem, t_alloc *alloc, int no_fork);
int						redirection_3(t_ast *elem, t_alloc *alloc, int no_fork);

/*
******************************** REDIRECTION ***********************************
*/

int						agreg_1(t_ast *elem, t_alloc *alloc, int no_fork);
int						agreg_2(t_ast *elem, t_alloc *alloc, int no_fork);
int						agreg_3(t_ast *elem, t_alloc *alloc, int no_fork);
int						agreg_4(t_ast *elem, t_alloc *alloc, int no_fork);
int						agreg_5(t_ast *elem, t_alloc *alloc, int no_fork);
int						ft_fd_exist(char *str_fd);
int						ft_is_redir(t_ast *elem, int *fd, t_alloc *alloc);
int						ft_is_agreg(t_ast *elem, int *fd, t_alloc *alloc);
int						heredoc_content(t_alloc *alloc, t_ast *elem, char *s);
int						get_last_index(int *i, char *s, int *save,
						char **input);
int						get_last_operator(int *i, char *s, int *save,
						char **input);
int						get_last_digit(int *i, char *s, int *save,
						char **input);
void					fill_last_elem(char **s, int i, int save, t_ast *new);
void					reinit_fd(int *fd, t_alloc *alloc);


#endif
