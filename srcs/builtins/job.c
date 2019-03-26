#include "shell.h"
#include "options.h"
#include "job.h"

static void	print_job_pipe(t_list *tmp)
{
	t_job	*job;
	char	*state;

	while (tmp)
	{
		job = tmp->content;
		state = single_job_state_str(job);
		if (job->state == STOPPED)
			ft_printf("      %s(%s) %d %s\n", state, status_stop_str(job->status), job->pid, job->cmd);
		else if (job->state == DONE && WEXITSTATUS(job->status))
			ft_printf("      %s(%d) %d %s\n", state, WEXITSTATUS(job->status), job->pid, job->cmd);
		else
			ft_printf("      %s %d %s\n", state, job->pid, job->cmd);
		tmp = tmp->next;
	}
}

static void	display_job_full(t_list *tmp, int index)
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
	state = single_job_state_str(job);
	if (job->state == STOPPED)
		ft_printf("[%d] %c %s(%s) %d %s\n", index, current, state, status_stop_str(job->status), job->gpid, job->cmd);
	else if (job->state == DONE && WEXITSTATUS(job->status))
		ft_printf("[%d] %c %s(%d) %d %s\n", index, current, state, WEXITSTATUS(job->status), job->gpid, job->cmd);
	else
		ft_printf("[%d] %c %s %d %s\n", index, current, state, job->gpid, job->cmd);
	if (job->pipe)
		print_job_pipe(job->pipe);
}

static void	display_option_job(t_list *tmp, t_opts *opts, int index, char *option_s)
{
	if (!has_opt(opts, 'l'))
		display_simple_job(tmp, index, opts);
	else if (!has_opt(opts, 'p'))
		display_job_full(tmp, index);
	else if (option_s && option_s[ft_strlen(option_s) - 1] == 'p')
		display_simple_job(tmp, index, opts);
	else
		display_job_full(tmp, index);
}

static int	display_jobs(t_opts *opts, char *option_s, int param)
{
	t_list	*tmp;
	int		index;

	index = 1;
	tmp = g_jobs;
	while (tmp)
	{
		if (param == index)
		{
			display_option_job(tmp, opts, index, option_s);
			break ;
		}
		else if (param == -1)
			display_option_job(tmp, opts, index, option_s);
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
	refresh_jobs();
	return (display_jobs(&opts, elem->input[opts.index - 1], param));
}
