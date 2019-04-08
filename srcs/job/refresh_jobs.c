/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refresh_jobs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:28:39 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/08 13:28:40 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include "shell.h"
#include "job.h"

static void	refresh_stop_state(t_job *job, int *print, int status, int ret)
{
	if (job->state == STOPPED || job->state == STOPPED_PENDING)
	{
		if (ret > 0 && WIFCONTINUED(status))
			job->state = RUNNING_BG;
		if (ret > 0 && (WIFSIGNALED(status) || WIFEXITED(status)))
		{
			ret_status(status, job->pid, job, 0);
			*print = 1;
		}
	}
}

static void	refresh_state_job(t_job *job, int *print, int *stop_print)
{
	int		ret;
	int		status;

	ret = 0;
	status = 0;
	if (job->state < SIG)
	{
		ret = waitpid(job->pid, &status, WNOHANG | WUNTRACED | WCONTINUED);
	}
	if (job->state == RUNNING_BG && ret > 0)
	{
		ret_status(status, job->pid, job, 0);
		if (job->state >= SIG)
			*print = 1;
		else if (job->state == STOPPED_PENDING)
		{
			*stop_print = 1;
			job->state = STOPPED;
		}
	}
	else if (job->state == STOPPED_PENDING && (!ret || WIFSTOPPED(status)))
	{
		job->state = STOPPED;
		*stop_print = 1;
	}
	else
		refresh_stop_state(job, print, status, ret);
}

void		refresh_state(t_list *tmp, int print_state)
{
	t_job	*job;
	int		print;
	int		index;
	int		stop_print;
	t_list	*pipe;

	index = 1;
	while (tmp)
	{
		print = 0;
		stop_print = 0;
		job = tmp->content;
		refresh_state_job(job, &print, &stop_print);
		pipe = job->pipe;
		while (pipe)
		{
			job = pipe->content;
			refresh_state_job(job, &print, &stop_print);
			pipe = pipe->next;
		}
		if (print_state)
			print_refreshed_jobs(tmp, print, stop_print, index);
		tmp = tmp->next;
		index += 1;
	}
}

void		refresh_jobs(void)
{
	t_list		*tmp;

	tmp = g_jobs;
	if (!tmp)
		return ;
	refresh_state(tmp, 1);
	delete_jobs_terminated(tmp);
}
