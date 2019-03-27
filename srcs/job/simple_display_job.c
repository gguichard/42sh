#include "shell.h"
#include "job.h"

void	display_simple_job(t_list *tmp, int index, t_opts *opts)
{
	char	current;
	t_job	*job;
	char	*state;
	char	*cmd;

	job = tmp->content;
	current = ' ';
	if (!tmp->next)
		current = '+';
	else if (!tmp->next->next)
		current = '-';
	state = job_state_str(tmp);
	cmd = job_cmd(job);
	if (opts && has_opt(opts, 'p'))
		ft_printf("%d\n", job->gpid);
	else if (job->pipe && !ft_strcmp("SIGTSTP", signal_stop_str(tmp)))
		ft_printf("[%d] %c %s %s\n", index, current, state, cmd);
	else if (job->state == STOPPED)
		ft_printf("[%d] %c %s(%s) %s\n", index, current, state, signal_stop_str(tmp), cmd);
	else if (job->state == DONE && last_pid_exit_status(job))
		ft_printf("[%d] %c %s(%d) %s\n", index, current, state, last_pid_exit_status(job), cmd);
	else
		ft_printf("[%d] %c %s %s\n", index, current, state, cmd);
	ft_memdel((void **)&cmd);
}
