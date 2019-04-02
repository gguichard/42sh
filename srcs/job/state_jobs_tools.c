#include "shell.h"
#include "job.h"

void	update_job_state(pid_t process, t_job_state state)
{
	t_list	*tmp;
	t_list	*prev;
	t_job	*job;

	prev = 0;
	tmp = g_jobs;
	while (tmp)
	{
		job = tmp->content;
		if (job->pid == process)
		{
			job->state = state;
			break ;
		}
		if (job->pipe)
		{
			prev = tmp;
			tmp = job->pipe;
		}
		else
			tmp = tmp->next;
		if (!tmp && prev)
			tmp = prev->next;
	}
}

t_job	*check_job_state(t_list *tmp, t_job_state state)
{
	t_job	*job;

	job = tmp->content;
	if (job->state == state)
		return (job);
	if (job->pipe)
	{
		tmp = job->pipe;
		while (tmp)
		{
			job = tmp->content;
			if (job->state == state)
				return (job);
			tmp = tmp->next;
		}
	}
	return (0);
}

int		job_state_done(t_list *tmp)
{
	t_job	*job;

	job = tmp->content;
	if (job->state < SIG)
		return (1);
	if (job->pipe)
	{
		tmp = job->pipe;
		while (tmp)
		{
			job = tmp->content;
			if (job->state < SIG)
				return (1);
			tmp = tmp->next;
		}
	}
	return (0);
}

int		job_state_run_or_done(t_list *tmp)
{
	t_job	*job;

	job = tmp->content;
	if (job->state < SIG && job->state != RUNNING_BG)
		return (1);
	if (job->pipe)
	{
		tmp = job->pipe;
		while (tmp)
		{
			job = tmp->content;
			if (job->state < SIG && job->state != RUNNING_BG)
				return (1);
			tmp = tmp->next;
		}
	}
	return (0);
}
