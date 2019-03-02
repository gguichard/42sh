/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/30 17:19:17 by tcollard          #+#    #+#             */
/*   Updated: 2019/02/20 12:19:20 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static int	process_pipe_left(t_ast *elem, t_env **lst_env, t_alloc **alloc)
{
	int	ret;

	close(elem->back->fd[0]);
	dup2(elem->back->fd[1], STDOUT_FILENO);
	close(elem->back->fd[1]);
	ret = analyzer(elem, lst_env, alloc);
	exit(ret);
}

static int	process_pipe_right(t_ast *elem, t_env **lst_env, t_alloc **alloc)
{
	int	ret;

	close(elem->back->fd[1]);
	dup2(elem->back->fd[0], STDIN_FILENO);
	close(elem->back->fd[0]);
	ret = analyzer(elem, lst_env, alloc);
	exit(ret);
}

int			do_pipe(t_ast *elem, t_env **lst_env, t_alloc **alloc)
{
	int	pid1;
	int	pid2;

	if (g_pid == -1 || pipe(elem->fd) == -1 || !elem->right || !elem->left)
		return (0);
	if (!(pid1 = fork()))
		process_pipe_left(elem->left, lst_env, alloc);
	else
	{
		g_pid = pid1;
		if (!(pid2 = fork()))
			process_pipe_right(elem->right, lst_env, alloc);
		else
		{
			g_pid = pid2;
			close(elem->fd[1]);
			close(elem->fd[0]);
			waitpid(pid1, &(g_ret[0]), 0);
			waitpid(pid2, &(g_ret[0]), 0);
			g_ret[1] = 1;
		}
	}
	return (1);
}
