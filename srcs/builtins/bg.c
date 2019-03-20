#include "shell.h"
#include "builtins.h"

int	bg_builtins(t_ast *elem, t_alloc *alloc)
{
	int		index;
	t_list	*tmp;
	t_job	*job;

	(void)alloc;
	index = 1;
	tmp = g_jobs;
	if (elem->input[1])
	{
		index = ft_atoi(elem->input[1]);
		while (--index > 0 && tmp)
			tmp = tmp->next;
	}
	else
	{
		while (tmp->next)
		{
			index += 1;
			tmp = tmp->next;
		}
	}
	if (tmp)
	{
		job = tmp->content;
		if (index < 1 && elem->input[1])
			index = ft_atoi(elem->input[1]);
		ft_printf("[%d] %s &\n", index, job->cmd);
		job->state = RUNNING;
		killpg(job->gpid, SIGCONT);
	}
	return (0);
}
