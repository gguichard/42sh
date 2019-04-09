/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 13:51:13 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/09 14:27:24 by gguichar         ###   ########.fr       */
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

static int	redirect_file(t_redirect_inf *redirect_inf)
{
	int		is_write;
	int		fd;
	t_error	error;

	is_write = (redirect_inf->red_type == RD_R
			|| redirect_inf->red_type == RD_RR);
	fd = open(redirect_inf->ropt_file
			, is_write ? (O_CREAT | O_WRONLY) : O_RDONLY, 0644);
	if (fd != -1)
		close(fd);
	else
	{
		error = check_file_rights(redirect_inf->ropt_file, FT_FILE
				, is_write ? W_OK : R_OK);
		if (is_write && error == ERRC_FILENOTFOUND)
			error = check_dir_of_file_rights(redirect_inf->ropt_file
					, X_OK | (is_write ? W_OK : R_OK));
		ft_dprintf(STDERR_FILENO, "42sh: %s: %s\n", redirect_inf->ropt_file
				, error_to_str(error));
		return (0);
	}
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
		else if (redirect_inf->ropt_fd == FD_NOTSET)
			return (redirect_file(redirect_inf));
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
