#include "shell.h"

void	actualize_job_status(t_list *tmp)
{
	t_list	*prev;
	t_list	*aft;
	t_job	*job;

	prev = 0;
	while (tmp)
	{
		job = tmp->content;
		aft = tmp->next;
		if (job->state == STOPPED_PENDING)
		{
			job->state = STOPPED;
			print_job(job->pid);
		}
		if (job->state == RUNNING_BG && waitpid(job->pid, &job->status, WNOHANG))
		{
			job->state = DONE;
			print_job(job->pid);
		}
		if (job->state == DONE && !job->pipe)
		{
			if (prev)
				prev->next = aft;
			else
				g_jobs = aft;
			ft_memdel((void **)&(job->cmd));
			ft_memdel((void **)&(tmp->content));
			ft_memdel((void **)&tmp);
			tmp = aft;
		}
		else
		{
			prev = tmp;
			tmp = tmp->next;
		}
	}
}

void	del_job_elem(void *content, size_t size)
{
	t_job	*job;

	(void)size;
	job = content;
	ft_memdel((void **)&job->cmd);
	ft_memdel((void **)&content);
}

void	actualize_pipe_job_status(t_list **main, t_list *prev)
{
	int		delete;
	t_job	*job;
	t_list	*tmp;

	delete = 1;
	job = (*main)->content;
	tmp = job->pipe;
	while (tmp)
	{
		job = tmp->content;
		if (job->state == STOPPED_PENDING || job->state == STOPPED)
		{
			delete = 0;
			if (job->state == STOPPED_PENDING)
				delete = -1;
			job->state = STOPPED;
		}
		else if (job->state == RUNNING_BG && waitpid(job->pid, &job->status, WNOHANG))
			job->state = DONE;
		tmp = tmp->next;
	}
	job = (*main)->content;
	if ((job->state == STOPPED_PENDING || job->state == STOPPED))
		delete = 0;
	if (delete == 1)
	{
		ft_lstdel(&(((t_job *)(*main)->content)->pipe), del_job_elem);
		if (prev)
			prev->next = (*main)->next;
		else
			g_jobs = (*main)->next;
		del_job_elem((*main)->content, (*main)->content_size);
		ft_memdel((void **)main);
	}
	if (delete == -1)
		print_job(((t_job *)(*main)->content)->pid);
}

void	refresh_jobs_finish(void)
{
	t_list	*tmp;
	t_list	*prev;
	t_list	*aft;
	t_job	*job;

	tmp = g_jobs;
	prev = 0;
	while (tmp)
	{
		job = tmp->content;
		aft = tmp->next;
		if (job->pipe)
			actualize_pipe_job_status(&tmp, prev);
		prev = tmp;
		tmp = aft;
	}
	tmp = g_jobs;
	actualize_job_status(tmp);
}
