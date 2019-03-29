/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 15:29:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/29 13:01:31 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "shell.h"
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
	int			should_save_fd;
	t_list		*save_fd;
}				t_exec_opt;

int				exec_input(t_alloc *alloc, t_ast *elem, t_exec_opt *opt);

int				analyzer(t_alloc *alloc, t_ast *elem, t_exec_opt *opt);

int				try_builtin_execution(t_alloc *alloc, t_ast *elem
		, t_exec_opt *opt);

int				dup2_with_rc(t_redirect_inf *redirect_inf, int lopt_fd
		, int ropt_fd);
int				close_with_rc(t_redirect_inf *redirect_inf, int fd);

int				redirect_output(t_redirect_inf *redirect_inf);
int				redirect_input(t_redirect_inf *redirect_inf);
int				redirect_heredoc(t_redirect_inf *redirect_inf);

void			process_redir(t_redirect_inf *redirect_inf);
int				setup_redirection(t_redirect_inf *redirect_inf);

#endif
