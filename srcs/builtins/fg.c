#include "shell.h"
#include "builtins.h"

int	fg_builtins(t_ast *elem, t_alloc *alloc)
{
	int		index;
	t_list	*tmp;
	t_job	*job;

	index = 0;
	tmp = g_jobs;
	if (elem->input[1])
		index = ft_atoi(elem->input[1]);
	else
	{
		while (tmp && tmp->next)
			tmp = tmp->next;
	}
	if (tmp)
	{
		job = tmp->content;
		ft_printf("%s\n", job->cmd);
		redirect_term_controller(job->pid, 0);
		kill(job->pid, SIGCONT);
		waitpid(job->pid, &alloc->ret_val, WUNTRACED);
		redirect_term_controller(0, 1);
	}
	if (!tmp)
		return (1);
	return (ret_status(alloc->ret_val, job->pid));
}
