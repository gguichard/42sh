#include "shell.h"
#include "builtins.h"

int	fg_builtins(t_ast *elem, t_alloc *alloc)
{
	int		index;
	char	*cmd;
	t_list	*tmp;
	t_job	*job;

	(void)alloc;
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
	cmd = job_cmd(job);
	ft_printf("%s\n", cmd);
	ft_memdel((void **)&cmd);
	tcsetpgrp(STDIN_FILENO, job->gpid);
	killpg(job->gpid, SIGCONT);
	if (job->pipe)
		return (waiting_line(false, tmp));
	waitpid(job->pid, &job->status, WUNTRACED);
	redirect_term_controller(0, 1);
	return (ret_status(job->status, job->pid, job));
}
