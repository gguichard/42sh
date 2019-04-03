/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 15:29:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/03 17:32:59 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "shell.h"
# include "error.h"
# include "redirect_inf.h"

typedef struct	s_redirect_save
{
	int			original_fd;
	int			to_fd;
}				t_redirect_save;

typedef struct	s_exec_opt
{
	int			fork;
	int			wait_hang;
	t_list		*red_save;
	int			from_builtin;
	int			sigint;
}				t_exec_opt;

int				process_assigns(t_alloc *alloc, t_ast *elem, t_exec_opt *opt);

int				do_pipe(t_alloc *alloc, t_ast *elem, t_exec_opt *opt);
pid_t			process_pipe_fork(t_alloc *alloc, t_ast *elem, int already_piped
		, int wait_hang);

int				waiting_line(t_list *tmp, int wait_hang
		, t_alloc *alloc, t_exec_opt *opt);
void			wait_pid(pid_t child, t_alloc *alloc, t_ast *elem
		, t_exec_opt *opt);

int				job_control(t_alloc *alloc, t_ast *elem, t_exec_opt *opt);

char			*exec_path(t_alloc *alloc, const char *name, int *hashable
		, t_error *err);
void			execute_cmd(t_alloc *alloc, char **argv, char *path_exec);
int				exec_input(t_alloc *alloc, t_ast *elem, t_exec_opt *opt);

int				analyzer(t_alloc *alloc, t_ast *elem, t_exec_opt *opt);

int				try_builtin_execution(t_alloc *alloc, t_ast *elem
		, t_exec_opt *opt);

void			use_rc_on_shell(t_exec_opt *opt);
int				dup2_with_rc(t_exec_opt *opt, int lopt_fd, int ropt_fd);
int				close_with_rc(t_exec_opt *opt, int fd);

int				redirect_output(t_redirect_inf *redirect_inf, t_exec_opt *opt);
int				redirect_input(t_redirect_inf *redirect_inf, t_exec_opt *opt);
int				redirect_heredoc(t_redirect_inf *redirect_inf, t_exec_opt *opt);

void			process_redir(t_redirect_inf *redirect_inf);
int				setup_redirection(t_redirect_inf *redirect_inf
		, t_exec_opt *opt);

t_error			source_file(t_alloc *alloc, const char *path);
void			source_rc_file(t_alloc *alloc);

#endif
