/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 13:51:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 11:07:21 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "execute.h"
#include "cmd_inf.h"
#include "redirect_inf.h"
#include "check_path.h"

static void	redirect_file(t_redirect_inf *redirect_inf)
{
	int		is_write;
	int		fd;
	t_error	error;

	if (redirect_inf->to_fd == FD_ERROR)
		ft_dprintf(2, "%s: -1: Bad file descriptor\n", ERR_PREFIX);
	else if (redirect_inf->to_fd == FD_NOTSET)
	{
		is_write = (redirect_inf->red_type == RD_R
				|| redirect_inf->red_type == RD_RR);
		fd = open(redirect_inf->to_word
				, is_write ? (O_CREAT | O_WRONLY) : O_RDONLY, 0644);
		if (fd >= 0)
			close(fd);
		else
		{
			error = check_file_rights(redirect_inf->to_word, FT_FILE
					, is_write ? W_OK : R_OK);
			if (is_write && error == ERRC_FILENOTFOUND)
				error = check_dir_of_file_rights(redirect_inf->to_word
						, X_OK | (is_write ? W_OK : R_OK));
			ft_dprintf(2, "%s: %s: %s\n", ERR_PREFIX, redirect_inf->to_word
					, error_to_str(error));
		}
	}
}

void		process_redir(t_shell *shell, t_cmd_inf *cmd_inf)
{
	t_list			*cur_redir;
	t_redirect_inf	*redirect_inf;

	cur_redir = cmd_inf->redirect_lst;
	while (cur_redir != NULL)
	{
		redirect_inf = (t_redirect_inf *)cur_redir->content;
		if (redirect_inf->red_type != RD_LL)
			redirect_file(redirect_inf);
		else
			prompt_heredoc(shell, redirect_inf);
		cur_redir = cur_redir->next;
	}
	if (cmd_inf->pipe_cmd != NULL)
		process_redir(shell, cmd_inf->pipe_cmd);
}

int			setup_redirections(t_cmd_inf *cmd_inf)
{
	t_list			*curr;
	t_redirect_inf	*redirect_inf;

	curr = cmd_inf->redirect_lst;
	while (curr != NULL)
	{
		redirect_inf = (t_redirect_inf *)curr->content;
		if (redirect_inf->from_fd == FD_ERROR
				|| redirect_inf->to_fd == FD_ERROR)
		{
			ft_dprintf(2, "%s: -1: File descriptor out of range\n", ERR_PREFIX);
			return (0);
		}
		if ((redirect_inf->red_type == RD_R || redirect_inf->red_type == RD_RR)
				&& !redirect_output(redirect_inf))
			return (0);
		else if (redirect_inf->red_type == RD_L
				&& !redirect_input(redirect_inf))
			return (0);
		else if (redirect_inf->red_type == RD_LL
				&& !redirect_heredoc(redirect_inf))
			return (0);
		curr = curr->next;
	}
	return (1);
}

int			reset_redirections(t_cmd_inf *cmd_inf)
{
	t_list			*curr;
	t_list			*curr_save;
	t_redirect_save	*redirect_save;

	curr = cmd_inf->redirect_lst;
	while (curr != NULL)
	{
		curr_save = ((t_redirect_inf *)curr->content)->red_save;
		while (curr_save != NULL)
		{
			redirect_save = (t_redirect_save *)curr_save->content;
			dup2(redirect_save->original_fd, redirect_save->to_fd);
			if (redirect_save->from_fd != -1)
				close(redirect_save->from_fd);
			close(redirect_save->original_fd);
			curr_save = curr_save->next;
		}
		curr = curr->next;
	}
	return (1);
}
