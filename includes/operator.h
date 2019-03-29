#ifndef OPERATOR_H
# define OPERATOR_H

# include "shell.h"
# include "execution.h"

/*
********************************** OPERATOR ************************************
*/

int		do_pipe(t_alloc *alloc, t_ast *elem, t_exec_opt *opt);
pid_t	process_fork(t_alloc *alloc, t_ast *elem, int already_piped
		, int wait_hang);
int		waiting_line(int wait_hang, t_list *tmp);
int		job_control(t_alloc *alloc, t_ast *elem, t_exec_opt *opt);

#endif
