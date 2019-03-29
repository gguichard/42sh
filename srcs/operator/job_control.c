#include "shell.h"
#include "operator.h"
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
			job->state = RUNNING_BG;
			ft_printf("[%d] %d\n", index, process);
			break ;
		}
		index += 1;
		tmp = tmp->next;
	}
}

int		job_control(t_ast *elem, t_alloc *alloc, t_exec_opt *opt)
{
	while (elem)
	{
		opt->wait_hang = true;
		analyzer(elem->left, alloc, opt);
		if (elem->right && elem->right->type == AST_JOB)
			elem = elem->right;
		else
			break;
	}
	if (elem->right)
	{
		opt->wait_hang = false;
		return (analyzer(elem->right, alloc, opt));
	}
	return (0);
}
