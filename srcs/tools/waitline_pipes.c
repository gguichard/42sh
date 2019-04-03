#include <sys/wait.h>
#include "shell.h"
#include "execution.h"
#include "job.h"

static void	pipe_waits(t_list *lst, int wait_opt)
{
	t_job	*job;

	job = lst->content;
	waitpid(job->pid, &job->status, wait_opt);
	lst = job->pipe;
	while (lst)
	{
		job = lst->content;
		waitpid(job->pid, &job->status, wait_opt);
		lst = lst->next;
	}
}

static int	get_ret_val(t_list *tmp, t_exec_opt *opt)
{
	t_job	*job;
	int		stop;
	int		ret;

	stop = 0;
	job = tmp->content;
	if (job->state < SIG)
		ret = ret_status(job->status, job->pid, job, opt);
	if (job->state == STOPPED_PENDING)
		stop = ret;
	tmp = job->pipe;
	while (tmp)
	{
		job = tmp->content;
		if (job->state < SIG)
			ret = ret_status(job->status, job->pid, job, opt);
		if (job->state == STOPPED_PENDING)
			stop = ret;
		tmp = tmp->next;
	}
	if (!stop)
		return (ret);
	return (stop);
}

int		waiting_line(t_list *tmp, int wait_hang, t_alloc *alloc, t_exec_opt *opt)
{
	if (!tmp)
	{
		tmp = g_jobs;
		while (tmp && ((t_job *)tmp->content)->state != RUNNING_FG)
			tmp = tmp->next;
		if (!tmp)
			return (1);
	}
	pipe_waits(tmp, WNOHANG);
	if (!wait_hang)
	{
		pipe_waits(tmp, WUNTRACED);
		redirect_term_controller(0, 1);
	}
	else
	{
		alloc->last_bg = ((t_job *)tmp->content)->pid;
		print_bg(((t_job *)tmp->content)->pid);
		return (0);
	}
	return (get_ret_val(tmp, opt));
}

void	wait_pid(pid_t child, t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	setpgid(child, 0);
	add_pid_lst(child, elem, 0);
	if (!opt->wait_hang)
	{
		redirect_term_controller(child, 0);
		waitpid(child, &alloc->ret_val, WUNTRACED);
		redirect_term_controller(0, 1);
	}
	else
	{
		alloc->last_bg = child;
		print_bg(child);
	}
}
