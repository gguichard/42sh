#include <stdlib.h>
#include <signal.h>
#include "shell.h"
#include "execution.h"
#include "job.h"

static void	close_pipe(t_ast *elem, int already_piped)
{
	if (already_piped)
	{
		close(elem->fd[1]);
		close(elem->fd[0]);
	}
	else if (elem->back != NULL && elem->back->type == AST_PIPE)
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
	while (tmp != NULL)
	{
		job = tmp->content;
		if (job->state == RUNNING_FG)
		{
			kill(job->pid, SIGINT);
			waitpid(job->pid, NULL, WNOHANG);
			job->state = DONE;
			if (job->pipe)
				tmp = job->pipe;
			else
				tmp = tmp->next;
		}
	}
}

static void	pipe_inout(t_ast *elem, int already_piped)
{
	if (!already_piped && elem->fd[0] == -1 && elem->fd[1] == -1
			&& pipe(elem->fd) == 0)
	{
		elem->left->fd[1] = elem->fd[1];
		if (elem->right->type == AST_PIPE)
			elem->right->left->fd[0] = elem->fd[0];
		else
			elem->right->fd[0] = elem->fd[0];
	}
}

int			do_pipe(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	int		already_piped;
	pid_t	last_child;

	already_piped = 0;
	while (elem != NULL)
	{
		pipe_inout(elem, already_piped);
		if ((last_child = process_fork(alloc, elem, already_piped
						, opt->wait_hang)) == -1)
		{
			kill_fg_pgid();
			return (1);
		}
		close_pipe(elem, already_piped);
		if (elem->right->type == AST_PIPE)
			elem = elem->right;
		else if (already_piped)
			break ;
		already_piped = 1;
	}
	return (waiting_line(opt->wait_hang, 0));
}
