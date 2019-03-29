/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 11:58:43 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/29 10:03:08 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "execute.h"

static void	redirect_close_fd(t_redirect_inf *redirect_inf)
{
	if (redirect_inf->from_fd >= 0)
		close_with_rc(redirect_inf, redirect_inf->from_fd);
	if (redirect_inf->from_fd == FD_AMPERSAND
			|| redirect_inf->from_fd == FD_DEFAULT)
		close_with_rc(redirect_inf, STDOUT_FILENO);
	if (redirect_inf->from_fd == FD_AMPERSAND)
		close_with_rc(redirect_inf, STDERR_FILENO);
}

static int	open_redirect_file(t_redirect_inf *redirect_inf, int append_to_file)
{
	int		fd;
	int		oflag;

	oflag = O_CREAT | O_WRONLY;
	if (append_to_file)
		oflag |= O_APPEND;
	else
		oflag |= O_TRUNC;
	fd = open(redirect_inf->to_word, oflag, 0644);
	return (fd);
}

static int	redirect_fd(t_redirect_inf *redirect_inf)
{
	if (redirect_inf->to_fd == FD_AMPERSAND)
	{
		if (redirect_inf->from_fd != 1 && redirect_inf->from_fd != FD_DEFAULT)
			ft_dprintf(STDERR_FILENO, "42sh: %s: Ambiguous redirect\n"
					, redirect_inf->to_word);
		else
		{
			redirect_inf->from_fd = FD_AMPERSAND;
			redirect_inf->to_fd = FD_NOTSET;
		}
	}
	if (redirect_inf->to_fd == FD_NOTSET)
	{
		return (open_redirect_file(redirect_inf
					, redirect_inf->red_type == RD_RR));
	}
	return (redirect_inf->to_fd);
}

static int	redirect_to_fd(t_redirect_inf *redirect_inf)
{
	int	fd;
	int	dup_ret;

	if ((fd = redirect_fd(redirect_inf)) < 0)
		return (0);
	dup_ret = 1;
	if (redirect_inf->from_fd >= 0)
		dup_ret = dup2_with_rc(redirect_inf, fd, redirect_inf->from_fd);
	else
	{
		if (redirect_inf->from_fd == FD_AMPERSAND
				|| redirect_inf->from_fd == FD_DEFAULT)
			dup_ret = dup2_with_rc(redirect_inf, fd, STDOUT_FILENO);
		if (dup_ret && redirect_inf->from_fd == FD_AMPERSAND)
			dup_ret = dup2_with_rc(redirect_inf, fd, STDERR_FILENO);
	}
	if (!dup_ret)
		ft_dprintf(STDERR_FILENO, "42sh: %d: Bad file descriptor\n", fd);
	if (redirect_inf->close_to_fd || redirect_inf->to_fd == FD_NOTSET)
		close(fd);
	return (dup_ret);
}

int			redirect_output(t_redirect_inf *redirect_inf)
{
	if (redirect_inf->close_to_fd && redirect_inf->to_fd == FD_DEFAULT)
	{
		redirect_close_fd(redirect_inf);
		return (1);
	}
	return (redirect_to_fd(redirect_inf));
}
