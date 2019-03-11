/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 13:12:56 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 11:20:26 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include <unistd.h>
# include "shell.h"
# include "libft.h"
# include "cmd_inf.h"
# include "redirect_inf.h"
# include "error.h"

typedef struct	s_pipe
{
	int			is_leftmost;
	int			is_rightmost;
	int			fildes[2];
	int			in_fd;
	int			out_fd;
	t_cmd_inf	*cmd_inf;
}				t_pipe;

void			child_exec_cmd_inf(t_shell *shell, t_cmd_inf *cmd_inf
		, const char *bin_path, char **arg_tab);
int				try_execute_builtin(t_shell *shell, t_cmd_inf *cmd_inf
		, char **arg_tab);
void			execute_all(t_shell *shell, t_list *all_sub_cmd);
void			execute_single_cmd(t_shell *shell, t_cmd_inf *cmd_inf);
void			execute_pipeline(t_shell *shell, t_cmd_inf *cmd_inf);

/*
** UTILS.
*/
char			*search_binary(t_shell *shell, const char *binary
		, t_error *error);
char			**arg_lst_to_tab(t_list *arg_lst);
void			print_sig_message(int status);

/*
** REDIRECTIONS.
*/
int				dup2_with_rc(t_redirect_inf *redirect_inf, int from_fd
		, int to_fd);
int				close_with_rc(t_redirect_inf *redirect_inf, int fd);

void			process_redir(t_shell *shell, t_cmd_inf *cmd_inf);
void			prompt_heredoc(t_shell *shell, t_redirect_inf *redirect_inf);

int				setup_redirections(t_cmd_inf *cmd_inf);
int				reset_redirections(t_cmd_inf *cmd_inf);

int				redirect_output(t_redirect_inf *redirect_inf);
int				redirect_input(t_redirect_inf *redirect_inf);
int				redirect_heredoc(t_redirect_inf *redirect_inf);

void			setup_pipe_redirections(t_list *cur);
void			wait_for_pipe_end(t_shell *shell);

/*
** COMMANDS.
*/
char			*get_cmd_inf_path(t_shell *shell, t_cmd_inf *cmd_inf
		, t_error *error);

#endif
