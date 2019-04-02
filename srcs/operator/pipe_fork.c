#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "job.h"
#include "execution.h"

static int		process_pipe_command(t_alloc *alloc, t_ast *elem)
{
	t_exec_opt	opt_new;

	ft_memset(&opt_new, 0, sizeof(t_exec_opt));
	opt_new.fork = 1;
	exit(analyzer(alloc, elem, &opt_new));
}

static void		pipe_inout(t_ast *elem, int type)
{
	if (elem->fd[0] != -1)
	{
		dup2(elem->fd[0], STDIN_FILENO);
		close(elem->fd[0]);
		if (type && elem->back->back != NULL)
			close(elem->back->back->fd[1]);
		else
			close(elem->back->fd[1]);
	}
	if (elem->fd[1] != -1)
	{
		dup2(elem->fd[1], STDOUT_FILENO);
		close(elem->fd[1]);
		close(elem->back->fd[0]);
	}
}

static pid_t	add_pid_pipe(t_ast *elem, int last_pipe_cmd, pid_t child
		, int wait_hang)
{
	static t_list	*first_cmd = NULL;
	int				ret;

	if ((elem->back == NULL || elem->back->type != AST_PIPE) && !last_pipe_cmd)
	{
		ret = setpgid(child, 0);
		if (!wait_hang)
			redirect_term_controller(child, 0);
		if (!(first_cmd = add_pid_lst(child, elem->left, 0)))
			return (-1);
	}
	else if (last_pipe_cmd)
	{
		ret = add_pid_lst_pipe(first_cmd, child, elem->right, 1);
		first_cmd = NULL;
	}
	else
		ret = add_pid_lst_pipe(first_cmd, child, elem->left, 1);
	return (ret == 0 ? child : -1);
}

pid_t			process_fork(t_alloc *alloc, t_ast *elem, int last_pipe_cmd
		, int wait_hand)
{
	pid_t	child;

	child = fork();
	if (child > 0)
		child = add_pid_pipe(elem, last_pipe_cmd, child, wait_hand);
	else if (child == 0)
	{
		if (!last_pipe_cmd)
		{
			pipe_inout(elem->left, 1);
			process_pipe_command(alloc, elem->left);
		}
		else
		{
			pipe_inout(elem->right, 0);
			process_pipe_command(alloc, elem->right);
		}
	}
	return (child);
}
