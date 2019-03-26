#ifndef OPERATOR_H
# define OPERATOR_H

# include "shell.h"
# include "error.h"

/*
********************************** OPERATOR ************************************
*/

int						do_pipe(t_ast *elem, t_alloc *alloc, t_exec_opt *opt);
pid_t					process_fork(t_ast *elem, t_alloc *alloc,
						int already_piped, bool wait_hand);
int						waiting_line(bool wait_hang, t_list *tmp);
int						job_control(t_ast *elem, t_alloc *alloc, t_exec_opt *opt);
int						redirection(t_ast *elem, t_alloc *alloc, t_exec_opt *opt);
int						heredoc(t_ast *elem, t_alloc *alloc);
int						complete_heredoc(t_ast *lst, t_alloc *alloc);

/*
******************************** REDIRECTION ***********************************
*/

int						redirection_1(t_ast *elem, t_alloc *alloc, t_exec_opt *opt);
int						redirection_2(t_ast *elem, t_alloc *alloc, t_exec_opt *opt);
int						redirection_3(t_ast *elem, t_alloc *alloc, t_exec_opt *opt);

/*
******************************** REDIRECTION ***********************************
*/

int						agreg_1(t_ast *elem, t_alloc *alloc, t_exec_opt *opt);
int						agreg_2(t_ast *elem, t_alloc *alloc, t_exec_opt *opt);
int						agreg_3(t_ast *elem, t_alloc *alloc, t_exec_opt *opt);
int						agreg_4(t_ast *elem, t_alloc *alloc, t_exec_opt *opt);
int						agreg_5(t_ast *elem, t_alloc *alloc, t_exec_opt *opt);
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
