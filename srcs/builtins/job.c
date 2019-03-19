#include "shell.h"
#include "builtins.h"

static char	*job_state_str(t_job_state state)
{
	if (state == STOPPED)
		return ("Stopped");
	else if (state == DONE)
		return ("Done");
	else if (state == RUNNING)
		return ("Running");
	return ("undefined");
}

static char	*signal_stop_str(int status)
{
	if (WSTOPSIG(status) == SIGTSTP)
		return ("SIGTSTP");
	else if (WSTOPSIG(status) == SIGSTOP)
		return ("SIGSTOP");
	else if (WSTOPSIG(status) == SIGTTIN)
		return ("SIGTTIN");
	else if (WSTOPSIG(status) == SIGTTOU)
		return ("SIGTTOU");
	return ("undefined");
}

static void	display_job(t_list *tmp, int index, int def)
{
	char	current;
	t_job	*job;
	char	*state;

	job = tmp->content;
	if (!tmp->next)
		current = '+';
	else if (!tmp->next->next)
		current = '-';
	else
		current = ' ';
	state = job_state_str(job->state);
	if (job->state == STOPPED)
	{
		if (!def)
			ft_printf("[%d] %c %s(%s) %s\n", index, current, state, signal_stop_str(job->status), job->cmd);
		else
			ft_printf("[%d] %c %s(%s) %d %s\n", index, current, state, signal_stop_str(job->status), job->gpid, job->cmd);
	}
	else if (job->state == DONE && WIFEXITED(job->status))
	{
		if (!def)
			ft_printf("[%d] %c %s(%d) %s\n", index, current, state, WIFEXITED(job->status), job->cmd);
		else
			ft_printf("[%d] %c %s(%d) %d %s\n", index, current, state, WIFEXITED(job->status), job->gpid, job->cmd);
	}
	else
	{
		if (!def)
			ft_printf("[%d] %c %s %s\n", index, current, state, job->cmd);
		else
			ft_printf("[%d] %c %s %d %s\n", index, current, state, job->gpid, job->cmd);
	}
}

t_job		*get_job_pid(pid_t process)
{
	t_list	*tmp;

	tmp = g_jobs;
	if (tmp)
	{
		while (tmp->next && ((t_job *)tmp->content)->pid != process)
			tmp = tmp->next;
		return (((t_job *)tmp->content));
	}
	return (0);
}

void		print_job(pid_t process)
{
	t_list	*tmp;
	int		index;

	index = 1;
	if (g_jobs)
	{
		tmp = g_jobs;
		while (tmp->next && ((t_job *)tmp->content)->pid != process)
		{
			tmp = tmp->next;
			index += 1;
		}
		display_job(tmp, index, 0);
	}
}

static int	display_job_gpid(void)
{
	t_list	*tmp;
	t_job	*job;

	tmp = g_jobs;
	while (tmp)
	{
		job = tmp->content;
		ft_printf("%d\n", job->gpid);
		tmp = tmp->next;
	}
	return (0);
}

int			job_builtins(t_ast *elem, t_alloc *alloc)
{
	int		gpid_display;
	t_list	*tmp;
	int		index;

	(void)alloc;
	if (!elem->input[1])
		gpid_display = 0;
	else if (ft_strchr(elem->input[1], 'p'))
		return (display_job_gpid());
	else if (ft_strchr(elem->input[1], 'l'))
		gpid_display = 1;
	index = 1;
	tmp = g_jobs;
	while (tmp)
	{
		display_job(tmp, index, gpid_display);
		index += 1;
		tmp = tmp->next;
	}
	return (0);
}
