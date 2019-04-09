/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 13:51:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/10 00:42:45 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "shell.h"
#include "check_path.h"
#include "redirect_inf.h"
#include "execution.h"

static int	redirect_tempfile(t_alloc *alloc, t_redirect_inf *redirect_inf)
{
	char	*tempfile_path;
	int		fd;

	tempfile_path = get_tempfile_path(alloc, "sh-thd");
	fd = -1;
	if (tempfile_path != NULL)
		fd = open(tempfile_path, O_CREAT | O_WRONLY, 0600);
	if (fd == -1)
	{
		free(tempfile_path);
		ft_dprintf(STDERR_FILENO, "42sh: unable to create heredoc temp file\n");
		return (0);
	}
	write(fd, redirect_inf->ropt_file, ft_strlen(redirect_inf->ropt_file));
	close(fd);
	free(redirect_inf->ropt_file);
	redirect_inf->ropt_file = tempfile_path;
	return (1);
}

int			setup_redirection(t_alloc *alloc, t_redirect_inf *redirect_inf)
{
	if (redirect_inf->lopt_fd == FD_ERROR || redirect_inf->ropt_fd == FD_ERROR)
	{
		ft_dprintf(STDERR_FILENO, "42sh: -1: file descriptor out of range\n");
		return (0);
	}
	else
	{
		if (redirect_inf->red_type == RD_LL)
			return (redirect_tempfile(alloc, redirect_inf));
	}
	return (1);
}

int			process_redirection(t_redirect_inf *redirect_inf, t_exec_opt *opt)
{
	if (redirect_inf->lopt_fd == FD_ERROR
			|| redirect_inf->ropt_fd == FD_ERROR)
	{
		ft_dprintf(STDERR_FILENO, "42sh: -1: file descriptor out of range\n");
		return (0);
	}
	if ((redirect_inf->red_type == RD_R || redirect_inf->red_type == RD_RR)
			&& !redirect_output(redirect_inf, opt))
		return (0);
	else if (redirect_inf->red_type == RD_L
			&& !redirect_input(redirect_inf, opt))
		return (0);
	else if (redirect_inf->red_type == RD_LL
			&& !redirect_heredoc(redirect_inf, opt))
		return (0);
	else
		return (1);
}
