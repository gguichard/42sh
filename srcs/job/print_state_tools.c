#include <signal.h>
#include <sys/wait.h>
#include "shell.h"
#include "job.h"

char	*status_stop_str(int status)
{
	if (WSTOPSIG(status) == SIGTSTP)
		return ("SIGTSTP");
	else if (WSTOPSIG(status) == SIGSTOP)
		return ("SIGSTOP");
	else if (WSTOPSIG(status) == SIGTTIN)
		return ("SIGTTIN");
	else if (WSTOPSIG(status) == SIGTTOU)
		return ("SIGTTOU");
	return ("undefined");
}

char	*signal_stop_str(t_list *tmp)
{
	t_job	*job;
	int		stopped;

	stopped = 0;
	job = tmp->content;
	if (job->state == STOPPED && WSTOPSIG(job->status) != SIGTSTP)
			return (status_stop_str(job->status));
	else if (job->state == STOPPED && WSTOPSIG(job->status) == SIGTSTP)
		stopped = 1;
	tmp = job->pipe;
	while (tmp)
	{
		job = tmp->content;
		if (job->state == STOPPED && WSTOPSIG(job->status) != SIGTSTP)
				return (status_stop_str(job->status));
		else if (job->state == STOPPED && WSTOPSIG(job->status) == SIGTSTP)
			stopped = 1;
		tmp = tmp->next;
	}
	if (stopped)
		return ("SIGTSTP");
	return (0);
}

char	*job_state_str(t_list *tmp)
{
	t_job	*job;

	if (check_job_state(tmp, STOPPED))
		return ("Stopped");
	else if ((job = check_job_state(tmp, SIG)))
		return (last_sig_process(tmp));
	else if (check_job_state(tmp, RUNNING_BG))
		return ("Running");
	else if (check_job_state(tmp, DONE))
		return ("Done");
	return ("undefined");
}

char	*single_job_state_str(t_job *job)
{
	if (job->state == STOPPED)
		return ("Stopped");
	else if (job->state == DONE)
		return ("Done");
	else if (job->state == RUNNING_BG)
		return ("Running");
	else if (job->state == SIG)
		return (sig_str(job->status));
	return ("undefined");
}

char	*job_cmd(t_job *job)
{
	char	*pipe_cmd;
	char	*prev;
	char	*actual;
	t_list	*tmp;

	if (job->pipe)
	{
		tmp = job->pipe;
		pipe_cmd = ft_strdup(job->cmd);
		while (tmp)
		{
			actual = ((t_job *)tmp->content)->cmd;
			prev = pipe_cmd;
			pipe_cmd = ft_strjoin(prev, actual);
			ft_memdel((void **)&prev);
			if (!pipe_cmd || !tmp->next)
				break ;
			tmp = tmp->next;
		}
	}
	else
		pipe_cmd = ft_strdup(job->cmd);
	return (pipe_cmd);
}
