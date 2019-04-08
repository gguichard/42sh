/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waitline_pipes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 15:34:42 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/08 19:39:51 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <signal.h>
#include "shell.h"
#include "execution.h"
#include "job.h"
#include "signals.h"

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
	t_list	*main;
	int		stop;
	int		ret;

	stop = 0;
	job = tmp->content;
	if (job->state < SIG)
		ret = ret_status(job->status, job->pid, job, opt);
	if (job->state == STOPPED_PENDING)
		stop = ret;
	main = tmp;
	tmp = job->pipe;
	while (tmp)
	{
		job = tmp->content;
		if (job->state < SIG)
			ret = ret_status(job->status, job->pid, job, opt);
		(job->state == STOPPED_PENDING) ? stop = ret : 0;
		tmp = tmp->next;
	}
	print_nl_sigint(main);
	if (!stop)
		return (ret);
	return (stop);
}

int			waiting_line(t_list *tmp, int wait_hang, t_alloc *alloc
			, t_exec_opt *opt)
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

void		kill_zombie_boy(pid_t boy, int display_err)
{
	kill(boy, SIGKILL);
	waitpid(boy, 0, WUNTRACED);
	if (display_err == 1)
		ft_dprintf(STDERR_FILENO
			, "42sh: child setpgid: operation not permitted\n");
}

void		wait_pid(pid_t child, t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	int		ret;

	ret = 0;
	if ((ret = setpgid(child, 0)) == -1)
		kill_zombie_boy(child, 1);
	else
		add_pid_lst(child, elem, 0);
	if (!opt->wait_hang && !ret)
	{
		redirect_term_controller(child, 0);
		waitpid(child, &alloc->ret_val, WUNTRACED);
		redirect_term_controller(0, 1);
	}
	else if (opt->wait_hang)
	{
		alloc->last_bg = child;
		print_bg(child);
	}
}
