#include "shell.h"
#include "execution.h"
#include "job.h"

void	print_bg(pid_t process)
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

int		job_control(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	while (elem)
	{
		opt->wait_hang = 1;
		analyzer(alloc, elem->left, opt);
		if (elem->right && elem->right->type == AST_JOB)
			elem = elem->right;
		else
			break;
	}
	if (elem->right)
	{
		opt->wait_hang = 0;
		return (analyzer(alloc, elem->right, opt));
	}
	return (0);
}
