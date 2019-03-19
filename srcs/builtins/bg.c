#include "shell.h"
#include "builtins.h"

int	bg_builtins(t_ast *elem, t_alloc *alloc)
{
	int		index;
	t_list	*tmp;
	t_job	*job;

	(void)alloc;
	index = 0;
	tmp = g_jobs;
	if (elem->input[1])
		index = ft_atoi(elem->input[1]);
	else
	{
		while (tmp->next)
			tmp = tmp->next;
	}
	while (--index > 0 && tmp)
		tmp = tmp->next;
	if (tmp)
	{
		job = tmp->content;
		kill(job->pid, SIGCONT);
	}
	return (0);
}
