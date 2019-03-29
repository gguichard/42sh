/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 13:51:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/29 10:01:34 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "execute.h"
#include "redirect_inf.h"
#include "check_path.h"

static void	redirect_file(t_redirect_inf *redirect_inf)
{
	int		is_write;
	int		fd;
	t_error	error;

	if (redirect_inf->to_fd == FD_ERROR)
		ft_dprintf(STDERR_FILENO, "42sh: -1: Bad file descriptor\n");
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
			ft_dprintf(STDERR_FILENO, "42sh: %s: %s\n", redirect_inf->to_word
					, error_to_str(error));
		}
	}
}

int			setup_redirections(t_redirect_inf *redirect_inf)
{
	if (redirect_inf->from_fd == FD_ERROR
			|| redirect_inf->to_fd == FD_ERROR)
	{
		ft_dprintf(STDERR_FILENO, "42sh: -1: File descriptor out of range\n");
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
	else
		return (1);
}
