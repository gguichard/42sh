#include <signal.h>
#include <sys/wait.h>
#include "shell.h"
#include "job.h"

static void	del_job_elem(void *content, size_t size)
{
	t_job	*job;

	(void)size;
	job = content;
	ft_memdel((void **)&job->cmd);
	ft_memdel((void **)&content);
}

static void	actualize_pipe_job_status(t_list **main, t_list *prev)
{
	int		delete;
	t_job	*job;
	t_list	*tmp;

	delete = 1;
	job = (*main)->content;
	if (job->state > RUNNING_FG && job->state < SIG)
		delete = 0;
	tmp = job->pipe;
	while (tmp)
	{
		job = tmp->content;
		if (job->state > RUNNING_FG && job->state < SIG)
			delete = 0;
		tmp = tmp->next;
	}
	if (delete == 1)
	{
		if (job->pipe)
			ft_lstdel(&(job->pipe), del_job_elem);
		if (prev)
			prev->next = (*main)->next;
		else
			g_jobs = (*main)->next;
		del_job_elem((*main)->content, (*main)->content_size);
		ft_memdel((void **)main);
	}
}

void		delete_jobs_terminated(t_list *tmp)
{
	t_list	*prev;
	t_list	*aft;

	prev = 0;
	while (tmp)
	{
		aft = tmp->next;
		actualize_pipe_job_status(&tmp, prev);
		prev = tmp;
		tmp = aft;
	}
}

void		terminate_all_jobs(void)
{
	t_list	*tmp;
	t_list	*pipe;
	t_job	*job;

	tmp = g_jobs;
	pipe = 0;
	while (tmp)
	{
		job = tmp->content;
		kill(job->pid, SIGTERM);
		waitpid(job->pid, 0, 0);
		job->state = DONE;
		if (job->pipe)
		{
			pipe = tmp;
			tmp = job->pipe;
		}
		else
			tmp = tmp->next;
		if (!tmp && pipe)
			tmp = pipe->next;
	}
	delete_jobs_terminated(g_jobs);
}
