#include "shell.h"
#include "options.h"
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

static void	display_one_job(t_list *tmp, int index, t_opts *opts)
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
	if (opts && has_opt(opts, 'p'))
		ft_printf("%d\n", job->gpid);
	else if (job->state == STOPPED)
	{
		if (!opts || !has_opt(opts, 'l'))
			ft_printf("[%d] %c %s(%s) %s\n", index, current, state, signal_stop_str(job->status), job->cmd);
		else
			ft_printf("[%d] %c %s(%s) %d %s\n", index, current, state, signal_stop_str(job->status), job->gpid, job->cmd);
	}
	else if (job->state == DONE && WIFEXITED(job->status))
	{
		if (!opts || !has_opt(opts, 'l'))
			ft_printf("[%d] %c %s(%d) %s\n", index, current, state, WIFEXITED(job->status), job->cmd);
		else
			ft_printf("[%d] %c %s(%d) %d %s\n", index, current, state, WIFEXITED(job->status), job->gpid, job->cmd);
	}
	else
	{
		if (!opts || !has_opt(opts, 'l'))
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
		display_one_job(tmp, index, 0);
	}
}

static int	display_jobs(t_opts *opts, int param)
{
	t_list	*tmp;
	int		index;

	index = 1;
	tmp = g_jobs;
	while (tmp)
	{
		if (param == index)
		{
			display_one_job(tmp, index, opts);
			break ;
		}
		else if (param == -1)
			display_one_job(tmp, index, opts);
		index += 1;
		tmp = tmp->next;
	}
	return (0);
}

int			job_builtins(t_ast *elem, t_alloc *alloc)
{
	int		param;
	t_opts	opts;

	(void)alloc;
	param = -1;
	parse_opts(&opts, elem->input, "lp");
	if (opts.error)
	{
		ft_dprintf(STDERR_FILENO, "42sh: jobs: -%c: invalid option\n", opts.error);
		ft_dprintf(STDERR_FILENO, "42sh: jobs: usage: jobs [-lp] [job_id ...]\n");
		return (2);
	}
	if (elem->input[opts.index])
		param = ft_atoi(elem->input[opts.index]);
	if (param > (int)ft_lstsize(g_jobs) || (param < 1 && elem->input[opts.index]))
	{
		ft_dprintf(STDERR_FILENO, "42sh: jobs: %s: no such job\n", elem->input[opts.index]);
		return (1);
	}
	return (display_jobs(&opts, param));
}
