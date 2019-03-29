/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 20:36:38 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/28 21:00:25 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "shell.h"
#include "execution.h"
#include "builtins.h"

static void	save_shell_fd(t_alloc *alloc)
{
	size_t	fd;

	fd = 0;
	while (fd < 10)
	{
		alloc->fd[fd] = fcntl(fd, F_DUPFD, 10);
		fd++;
	}
}

static void	restore_shell_fd(t_alloc *alloc)
{
	size_t	fd;

	fd = 0;
	while (fd < 10)
	{
		if (alloc->fd[fd] != -1)
		{
			dup2(alloc->fd[fd], fd);
			close(alloc->fd[fd]);
			alloc->fd[fd] = -1;
		}
		fd++;
	}
}

int			try_builtin_execution(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	size_t	idx;
	int		ret;

	idx = 0;
	while (alloc->builtins[idx].name != NULL)
	{
		if (ft_strequ(elem->input[0], alloc->builtins[idx].name))
		{
			if (elem->left != NULL)
			{
				save_shell_fd(alloc);
				analyzer(elem->left, alloc, opt);
			}
			ret = alloc->builtins[idx].built_fun(elem, alloc);
			if (elem->left != NULL)
				restore_shell_fd(alloc);
			return (ret);
		}
		idx++;
	}
	return (-1);
}
