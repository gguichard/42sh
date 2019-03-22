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
		while (tmp && tmp->next)
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
		if (job->state == STOPPED)
			job->state = RUNNING_BG;
		killpg(job->gpid, SIGCONT);
		if (job->pipe)
		{
			tmp = job->pipe;
			while (tmp)
			{
				job = tmp->content;
				if (job->state == STOPPED)
					job->state = RUNNING_BG;

			}
		}
	}
	else
	{
		if (!elem->input[1])
			ft_dprintf(STDERR_FILENO, "42sh: bg: no current job\n");
		else
			ft_dprintf(STDERR_FILENO, "42sh: bg: job not found: %s\n", elem->input[1]);
		return (1);
	}
	return (0);
}
