#include "shell.h"
#include "options.h"
#include "builtins.h"

static char	*status_stop_str(int status)
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

static char	*signal_stop_str(t_list *tmp)
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

static char	*job_state_str(t_list *tmp)
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

static char	*single_job_state_str(t_job *job)
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

static void	print_job_pipe(t_list *tmp)
{
	t_job	*job;
	char	*state;

	while (tmp)
	{
		job = tmp->content;
		state = single_job_state_str(job);
		if (job->state == STOPPED)
			ft_printf("      %s(%s) %d %s\n", state, status_stop_str(job->status), job->pid, job->cmd);
		else if (job->state == DONE && WEXITSTATUS(job->status))
			ft_printf("      %s(%d) %d %s\n", state, WEXITSTATUS(job->status), job->pid, job->cmd);
		else
			ft_printf("      %s %d %s\n", state, job->pid, job->cmd);
		tmp = tmp->next;
	}
}

static void	display_job_full(t_list *tmp, int index)
{
	char	current;
	t_job	*job;
	char	*state;

	job = tmp->content;
	if (!tmp->next)
		current = '+';
	else if (!tmp->next->next)
		current = '-';
	else
		current = ' ';
	state = single_job_state_str(job);
	if (job->state == STOPPED)
		ft_printf("[%d] %c %s(%s) %d %s\n", index, current, state, status_stop_str(job->status), job->gpid, job->cmd);
	else if (job->state == DONE && WEXITSTATUS(job->status))
		ft_printf("[%d] %c %s(%d) %d %s\n", index, current, state, WEXITSTATUS(job->status), job->gpid, job->cmd);
	else
		ft_printf("[%d] %c %s %d %s\n", index, current, state, job->gpid, job->cmd);
	if (job->pipe)
		print_job_pipe(job->pipe);
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

static int	last_pid_exit_status(t_job *job)
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

static void	display_simple_job(t_list *tmp, int index, t_opts *opts)
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

static void	display_option_job(t_list *tmp, t_opts *opts, int index, char *option_s)
{
	if (!has_opt(opts, 'l'))
		display_simple_job(tmp, index, opts);
	else if (!has_opt(opts, 'p'))
		display_job_full(tmp, index);
	else if (option_s && option_s[ft_strlen(option_s) - 1] == 'p')
		display_simple_job(tmp, index, opts);
	else
		display_job_full(tmp, index);
}

static int	display_jobs(t_opts *opts, char *option_s, int param)
{
	t_list	*tmp;
	int		index;

	index = 1;
	tmp = g_jobs;
	while (tmp)
	{
		if (param == index)
		{
			display_option_job(tmp, opts, index, option_s);
			break ;
		}
		else if (param == -1)
			display_option_job(tmp, opts, index, option_s);
		index += 1;
		tmp = tmp->next;
	}
	return (0);
}

int			job_builtins(t_ast *elem, t_alloc *alloc)
{
	int		param;
	t_opts	opts;

	(void)alloc;
	param = -1;
	parse_opts(&opts, elem->input, "lp");
	if (opts.error)
	{
		ft_dprintf(STDERR_FILENO, "42sh: jobs: -%c: invalid option\n", opts.error);
		ft_dprintf(STDERR_FILENO, "42sh: jobs: usage: jobs [-lp] [job_id ...]\n");
		return (2);
	}
	if (elem->input[opts.index])
		param = ft_atoi(elem->input[opts.index]);
	if (param > (int)ft_lstsize(g_jobs) || (param < 1 && elem->input[opts.index]))
	{
		ft_dprintf(STDERR_FILENO, "42sh: jobs: %s: no such job\n", elem->input[opts.index]);
		return (1);
	}
	refresh_jobs(true);
	return (display_jobs(&opts, elem->input[opts.index - 1], param));
}
