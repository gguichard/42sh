#include "shell.h"
#include "operator.h"
#include "job.h"

static void	close_pipe(t_ast *elem, int already_piped)
{
	if (already_piped)
	{
		close(elem->fd[1]);
		close(elem->fd[0]);
	}
	else if (elem->back && elem->back->type == AST_PIPE)
	{
		close(elem->back->fd[1]);
		close(elem->back->fd[0]);
	}
}

static void	kill_fg_pgid(void)
{
	t_list	*tmp;
	t_job	*job;

	tmp = g_jobs;
	while (tmp)
	{
		job = tmp->content;
		if (job->state == RUNNING_FG)
		{
			kill(job->pid, SIGINT);
			waitpid(job->pid, 0, WNOHANG);
			job->state = DONE;
			if (job->pipe)
				tmp = job->pipe;
			else
				tmp = tmp->next;
		}
	}
}

static void	set_connection(t_ast *elem, int already_piped)
{
	if (!already_piped && elem->fd[0] == -1 && elem->fd[1] == -1 && !pipe(elem->fd))
	{
		if (elem->right->type == AST_PIPE)
			elem->right->left->fd[0] = elem->fd[0];
		else
			elem->right->fd[0] = elem->fd[0];
		elem->left->fd[1] = elem->fd[1];
	}
}

int			do_pipe(t_ast *elem, t_alloc *alloc, t_exec_opt *opt)
{
	int		already_piped;
	pid_t	last_child;

	already_piped = 0;
	while (elem)
	{
		set_connection(elem, already_piped);
		if ((last_child = process_fork(elem, alloc, already_piped, opt->wait_hang)) == -1)
			break ;
		close_pipe(elem, already_piped);
		if (elem->right->type != AST_PIPE)
		{
			if (already_piped)
				break ;
			already_piped = 1;
		}
		else
			elem = elem->right;
	}
	if (last_child == -1)
		kill_fg_pgid();
	else
		return (waiting_line(opt->wait_hang, 0));
	return (1);
}
