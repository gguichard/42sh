#include "shell.h"
#include "builtins.h"

int	job_builtins(t_ast *elem, t_alloc *alloc)
{
	t_list	*tmp;
	t_job	*job;
	char	*state;
	char	current;
	int		index;

	(void)elem;
	(void)alloc;
	state = "undefined";
	index = 1;
	tmp = g_jobs;
	while (tmp)
	{
		job = tmp->content;
		if (!tmp->next)
			current = '+';
		else if (!tmp->next->next)
			current = '-';
		else
			current = ' ';
		if (job->state == RUNNING)
			state = "RUNNING";
		else if (job->state == DONE)
			state = "DONE";
		else if (job->state == STOPPED)
			state = "STOPPED";
		ft_printf("[%d] %c %s %s\n", index, current, state, job->cmd);
		index += 1;
		tmp = tmp->next;
	}
	return (0);
}
