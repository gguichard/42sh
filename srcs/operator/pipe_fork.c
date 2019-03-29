#include <stdlib.h>
#include "shell.h"
#include "job.h"
#include "execution.h"

static int		process_pipe(t_alloc *alloc, t_ast *elem)
{
	t_exec_opt	opt_new;

	opt_new.fork = 1;
	opt_new.wait_hang = 0;
	exit(analyzer(alloc, elem, &opt_new));
}

static void		redir_pipe(t_ast *elem, int type)
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

static pid_t	add_pid_pipe(t_ast *elem, int already_piped, pid_t child
		, int wait_hang)
{
	static t_list	*first_cmd;
	int				ret;

	if ((elem->back == NULL || elem->back->type != AST_PIPE) && !already_piped)
	{
		ret = setpgid(child, 0);
		if (!wait_hang)
			redirect_term_controller(child, 0);
		first_cmd = add_pid_lst(child, elem->left, 0);
	}
	else if (already_piped)
		ret = add_pid_lst_pipe(first_cmd, child, elem->right, 1);
	else
		ret = add_pid_lst_pipe(first_cmd, child, elem->left, 1);
	if (!ret)
		return (child);
	else
		return (-1);
}

pid_t			process_fork(t_alloc *alloc, t_ast *elem, int already_piped
		, int wait_hand)
{
	pid_t	child;

	child = fork();
	if (child > 0)
		child = add_pid_pipe(elem, already_piped, child, wait_hand);
	else if (child == 0)
	{
		if (!already_piped)
		{
			redir_pipe(elem->left, 1);
			process_pipe(alloc, elem->left);
		}
		else
		{
			redir_pipe(elem->right, 0);
			process_pipe(alloc, elem->right);
		}
	}
	return (child);
}
