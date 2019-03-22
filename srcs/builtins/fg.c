#include "shell.h"
#include "builtins.h"

int	fg_builtins(t_ast *elem, t_alloc *alloc)
{
	int		index;
	t_list	*tmp;
	t_job	*job;

	index = -1;
	tmp = g_jobs;
	if (elem->input[1])
		index = ft_atoi(elem->input[1]);
	if (!tmp || (index > (int)ft_lstsize(tmp)) || (index < 1 && elem->input[1]))
	{
		if (!elem->input[1])
			ft_dprintf(STDERR_FILENO, "42sh: fg: no current job\n");
		else
			ft_dprintf(STDERR_FILENO, "42sh: fg: job not found: %s\n", elem->input[1]);
		return (1);
	}
	if (index == -1)
	{
		while (tmp && tmp->next)
			tmp = tmp->next;
	}
	else
	{
		while (tmp && --index > 0)
			tmp = tmp->next;
	}
	job = tmp->content;
	ft_printf("%s\n", job->cmd);
	redirect_term_controller(job->gpid, 0);
	killpg(job->gpid, SIGCONT);
	if (job->pipe)
		return (waiting_line(false, tmp));
	waitpid(job->pid, &alloc->ret_val, WUNTRACED);
	redirect_term_controller(0, 1);
	return (ret_status(alloc->ret_val, job->pid, 0));
}
