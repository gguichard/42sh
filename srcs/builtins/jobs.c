/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:27:15 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/08 13:27:16 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
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
		else if (job->state == DONE && job->status)
			ft_printf("      %s(%d) %d %s\n", state, job->status, job->pid, job->cmd);
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
	else if (job->state == DONE && job->status)
		ft_printf("[%d] %c %s(%d) %d %s\n", index, current, state, job->status, job->gpid, job->cmd);
	else
		ft_printf("[%d] %c %s %d %s\n", index, current, state, job->gpid, job->cmd);
	if (job->pipe)
		print_job_pipe(job->pipe);
}

static void	display_option_job(t_list *tmp, t_opts *opts, int index)
{
	if (has_opt(opts, 'l'))
		display_job_full(tmp, index);
	else if (has_opt(opts, 'p'))
		ft_printf("%d\n", ((t_job *)tmp->content)->gpid);
	else
		display_simple_job(tmp, index);
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
			display_option_job(tmp, opts, index);
			break ;
		}
		else if (param == -1)
			display_option_job(tmp, opts, index);
		index += 1;
		tmp = tmp->next;
	}
	delete_jobs_terminated(g_jobs);
	return (0);
}

int			builtin_jobs(t_ast *elem, t_alloc *alloc)
{
	int		param;
	t_opts	opts;

	(void)alloc;
	param = -1;
	parse_opts(&opts, elem->input, "{lp}");
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
	refresh_state(g_jobs, 0);
	return (display_jobs(&opts, param));
}
