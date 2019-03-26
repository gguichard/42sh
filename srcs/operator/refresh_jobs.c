#include "shell.h"

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

 char	*state_str(t_job_state state)
{
	if (state == STOPPED)
		return ("Stopped");
	else if (state == DONE)
		return ("Done");
	else if (state == RUNNING_BG)
		return ("Running");
	else if (state == SIG)
		return ("Signaled");
	return ("undefined");
}

void	refresh_state_job(t_job *job, int *print, int *stop_print)
{
	int		ret;

	if (job->state < SIG)
		ret = waitpid(job->pid, &job->status, WNOHANG);
	if (job->state == RUNNING_BG && ret > 0)
	{
		ret_status(job->status, job->pid, job);
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
		if (WIFEXITED(job->status) || WIFSIGNALED(job->status))
		{
			ret_status(job->status, job->pid, job);
			*print = 1;
		}
		else if (WIFSTOPPED(job->status))
		{
			if (job->state == STOPPED_PENDING)
				*stop_print = 1;
			job->state = STOPPED;
		}
	}
}

static int	job_state_done(t_list *tmp)
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

int	job_state_run_or_done(t_list *tmp)
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

char	*last_sig_process(t_list *tmp)
{
	t_job	*job;
	t_job	*last;

	last = 0;
	job = tmp->content;
	if (job->state == SIG)
		last = job;
	if (job->pipe)
	{
		tmp = job->pipe;
		while (tmp)
		{
			job = tmp->content;
			if (job->state == SIG)
				last = job;
			tmp = tmp->next;
		}
	}
	if (last)
		return (sig_str(last->status));
	return (0);
}

void	print_refreshed_jobs(t_list *tmp, int print, int stop_print, int index)
{
	// char	*cmd;

	(void)index;
	if (stop_print)
		print_job(((t_job *)tmp->content)->pid, 1);
	else if (!print && check_job_state(tmp, SIG) && !job_state_done(tmp))
		ft_dprintf(2, "%s\n", last_sig_process(tmp));
	else if (print && !job_state_done(tmp))
		print_job(((t_job *)tmp->content)->pid, 0);
	// else if (print && !job_state_run_or_done(tmp))
	// {
	// 	cmd = job_cmd(tmp->content);
	// 	ft_printf("[%d] %s : now running in background\n", index, cmd);
	// 	ft_memdel((void **)&cmd);
	// }
}

void	refresh_state(t_list *tmp, bool print_job)
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
		if (print_job == true)
			print_refreshed_jobs(tmp, print, stop_print, index);
		tmp = tmp->next;
		index += 1;
	}
}

void	delete_jobs_terminated(t_list *tmp)
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

void	refresh_jobs(bool print_job)
{
	t_list	*tmp;

	tmp = g_jobs;
	if (!tmp)
		return ;
	refresh_state(tmp, print_job);
	delete_jobs_terminated(tmp);
}
