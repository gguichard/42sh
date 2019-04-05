#include <stdlib.h>
#include "shell.h"
#include "execution.h"
#include "job.h"

void		print_bg(pid_t process)
{
	int		index;
	t_job	*job;
	t_list	*tmp;

	index = 1;
	tmp = g_jobs;
	while (tmp)
	{
		job = tmp->content;
		if (job->pid == process)
		{
			update_job_state(tmp, RUNNING_BG);
			ft_printf("[%d] %d\n", index, process);
			break ;
		}
		index += 1;
		tmp = tmp->next;
	}
}

static void	job_fork(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	pid_t	child;
	int		ret;

	child = fork();
	if (child == -1)
		ft_dprintf(STDERR_FILENO
			, "42sh: fork: resource temporarily unavailable\n");
	else if (child > 0)
	{
		if (setpgid(child, 0) == -1)
			kill_zombie_boy(child);
		else
		{
			add_pid_lst(child, elem, 0);
			print_bg(child);
		}
	}
	else
	{
		opt->fork = 1;
		ret = analyzer(alloc, elem, opt);
		exit(ret);
	}
}

int			job_control(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	while (elem)
	{
		opt->wait_hang = 1;
		if (elem->left && elem->left->type != AST_PIPE)
			job_fork(alloc, elem->left, opt);
		else
			analyzer(alloc, elem->left, opt);
		if (elem->right && elem->right->type == AST_JOB)
			elem = elem->right;
		else
			break ;
	}
	if (elem->right)
	{
		opt->wait_hang = 0;
		return (analyzer(alloc, elem->right, opt));
	}
	return (0);
}
