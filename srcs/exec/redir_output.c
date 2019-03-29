/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_output.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 11:58:43 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/29 15:26:26 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "redirect_inf.h"
#include "execution.h"

static void	redirect_close_fd(t_redirect_inf *redirect_inf, t_exec_opt *opt)
{
	if (redirect_inf->lopt_fd >= 0)
		close_with_rc(opt, redirect_inf->lopt_fd);
	if (redirect_inf->lopt_fd == FD_AMPERSAND
			|| redirect_inf->lopt_fd == FD_DEFAULT)
		close_with_rc(opt, STDOUT_FILENO);
	if (redirect_inf->lopt_fd == FD_AMPERSAND)
		close_with_rc(opt, STDERR_FILENO);
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
	fd = open(redirect_inf->ropt_file, oflag, 0644);
	return (fd);
}

static int	redirect_fd(t_redirect_inf *redirect_inf)
{
	if (redirect_inf->ropt_fd == FD_AMPERSAND)
	{
		if (redirect_inf->lopt_fd != 1 && redirect_inf->lopt_fd != FD_DEFAULT)
			ft_dprintf(STDERR_FILENO, "42sh: %s: ambiguous redirect\n"
					, redirect_inf->ropt_file);
		else
		{
			redirect_inf->lopt_fd = FD_AMPERSAND;
			redirect_inf->ropt_fd = FD_NOTSET;
		}
	}
	if (redirect_inf->ropt_fd == FD_NOTSET)
	{
		return (open_redirect_file(redirect_inf
					, redirect_inf->red_type == RD_RR));
	}
	return (redirect_inf->ropt_fd);
}

static int	redirect_ropt_fd(t_redirect_inf *redirect_inf, t_exec_opt *opt)
{
	int	fd;
	int	dup_ret;

	if ((fd = redirect_fd(redirect_inf)) < 0)
		return (0);
	dup_ret = 1;
	if (redirect_inf->lopt_fd >= 0)
		dup_ret = dup2_with_rc(opt, fd, redirect_inf->lopt_fd);
	else
	{
		if (redirect_inf->lopt_fd == FD_AMPERSAND
				|| redirect_inf->lopt_fd == FD_DEFAULT)
			dup_ret = dup2_with_rc(opt, fd, STDOUT_FILENO);
		if (dup_ret && redirect_inf->lopt_fd == FD_AMPERSAND)
			dup_ret = dup2_with_rc(opt, fd, STDERR_FILENO);
	}
	if (!dup_ret)
		ft_dprintf(STDERR_FILENO, "42sh: %d: bad file descriptor\n", fd);
	if (redirect_inf->close_ropt_fd || redirect_inf->ropt_fd == FD_NOTSET)
		close(fd);
	return (dup_ret);
}

int			redirect_output(t_redirect_inf *redirect_inf, t_exec_opt *opt)
{
	if (redirect_inf->close_ropt_fd && redirect_inf->ropt_fd == FD_DEFAULT)
	{
		redirect_close_fd(redirect_inf, opt);
		return (1);
	}
	return (redirect_ropt_fd(redirect_inf, opt));
}
