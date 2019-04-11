/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_job_state.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:27:47 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/11 13:56:55 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <unistd.h>
#include "shell.h"
#include "execution.h"
#include "job.h"

int		ret_status(int ret, pid_t process, t_job *job, t_exec_opt *opt)
{
	if (!job && WIFSIGNALED(ret) && WTERMSIG(ret) == SIGINT)
		write(1, "\n", 1);
	if ((!job && !(job = get_job_pid(process)))
			|| (g_sig && g_sig != SIGINT))
		return (1);
	if (WIFEXITED(ret))
	{
		job->state = DONE;
		job->status = WEXITSTATUS(ret);
		return (job->status);
	}
	else if (WIFSTOPPED(ret))
	{
		job->state = STOPPED_PENDING;
		job->status = WSTOPSIG(ret);
	}
	else if (WIFSIGNALED(ret))
	{
		job->state = SIG;
		job->status = WTERMSIG(ret);
		if (job->status == SIGINT && opt)
			opt->sigint = 1;
	}
	return (job->status + 128);
}
