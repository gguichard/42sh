#include <sys/wait.h>
#include <signal.h>
#include "shell.h"
#include "job.h"

static void	refresh_state_job(t_job *job, int *print, int *stop_print)
{
	int		ret;
	int		status;

	ret = 0;
	status = 0;
	if (job->state < SIG)
		ret = waitpid(job->pid, &status, WNOHANG | WUNTRACED);
	if (job->state == RUNNING_BG && ret > 0)
	{
		ret_status(status, job->pid, job);
		if (job->state >= SIG)
			*print = 1;
		else if (job->state == STOPPED_PENDING)
		{
			*stop_print = 1;
			job->state = STOPPED;
		}
	}
	else if (job->state == STOPPED || job->state == STOPPED_PENDING)
	{
		if (ret > 0 && (WIFSIGNALED(status) || WIFEXITED(status)))
		{
			ret_status(status, job->pid, job);
			*print = 1;
		}
		else if (!ret || WIFSTOPPED(status))
		{
			if (job->state == STOPPED_PENDING)
				*stop_print = 1;
			job->state = STOPPED;
		}
	}
	if (job->state == STOPPED)
	{
		kill(job->pid, SIGSTOP);
		ret = waitpid(job->pid, &status, WUNTRACED | WNOHANG);
		if (ret > 0)
		{
			job->state = RUNNING_BG;
			kill(job->pid, SIGCONT);
		}
	}
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
