#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "execution.h"
#include "job.h"

static void	close_pipe(t_ast *elem, int last_pipe_cmd)
{
	if (last_pipe_cmd)
	{
		close(elem->fd[0]);
		close(elem->fd[1]);
	}
	else if (elem->back != NULL && elem->back->type == AST_PIPE)
	{
		close(elem->back->fd[0]);
		close(elem->back->fd[1]);
	}
}

static int	setup_pipe_inout(t_ast *elem, int last_pipe_cmd)
{
	if (!last_pipe_cmd && elem->fd[0] == -1 && elem->fd[1] == -1)
	{
		if (pipe(elem->fd) == -1)
		{
			ft_dprintf(STDERR_FILENO
					, "42sh: pipe: resource temporarily unavailable\n");
			return (0);
		}
		elem->left->fd[1] = elem->fd[1];
		if (elem->right->type != AST_PIPE)
			elem->right->fd[0] = elem->fd[0];
		else
			elem->right->left->fd[0] = elem->fd[0];
	}
	return (1);
}

int			do_pipe(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	int		last_pipe_cmd;
	pid_t	last_child;

	last_pipe_cmd = 0;
	while (elem != NULL && elem->right != NULL
			&& (elem->right->type != AST_PIPE || elem->right->left != NULL))
	{
		if (!setup_pipe_inout(elem, last_pipe_cmd))
			break ;
		last_child = process_pipe_fork(alloc, elem, last_pipe_cmd
				, opt->wait_hang);
		close_pipe(elem, last_pipe_cmd);
		if (last_child == -1)
			break ;
		if (elem->right->type == AST_PIPE)
			elem = elem->right;
		else if (last_pipe_cmd)
			break ;
		else
			last_pipe_cmd = 1;
	}
	return (waiting_line(opt->wait_hang, 0));
}
