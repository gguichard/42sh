#include "shell.h"
#include "job.h"

int		last_pid_exit_status(t_job *job)
{
	t_list	*tmp;

	tmp = job->pipe;
	if (tmp && !tmp->next)
		job = tmp->content;
	while (tmp && tmp->next)
	{
		tmp = tmp->next;
		job = tmp->content;
	}
	return (WEXITSTATUS(job->status));
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

void		print_job(pid_t process, int after_signal)
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
		if (after_signal)
			write(STDOUT_FILENO, "\n", 1);
		display_simple_job(tmp, index, 0);
	}
}