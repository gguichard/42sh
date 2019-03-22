#include "shell.h"
#include "parser_lexer.h"

int				replace_val_ret(char **str, int i, int x, int err)
{
	char	*value;

	ft_delete_inside(str, i, x);
	value = ft_itoa(err);
	replace_str(str, value, i);
	ft_memdel((void **)&value);
	return (0);
}

static void	print_sig_message_2(int signal)
{
	if (signal == SIGFPE)
		ft_dprintf(2, "Floating point exception: %d\n", signal);
	else if (signal == SIGKILL)
		ft_dprintf(2, "Killed: %d\n", signal);
	else if (signal == SIGBUS)
		ft_dprintf(2, "Bus error: %d\n", signal);
	else if (signal == SIGSEGV)
		ft_dprintf(2, "Segmentation fault: %d\n", signal);
	else if (signal == SIGSYS)
		ft_dprintf(2, "Bad system call: %d\n", signal);
	else if (signal == SIGALRM)
		ft_dprintf(2, "Alarm clock: %d\n", signal);
	else if (signal == SIGTERM)
		ft_dprintf(2, "Terminated: %d\n", signal);
}

static void	print_sig_message(int status)
{
	int	signal;

	signal = WTERMSIG(status);
	if (signal == SIGHUP)
		ft_dprintf(2, "Hangup: %d\n", signal);
	else if (signal == SIGQUIT)
		ft_dprintf(2, "Quit: %d\n", signal);
	else if (signal == SIGILL)
		ft_dprintf(2, "Illegal instruction: %d\n", signal);
	else if (signal == SIGTRAP)
		ft_dprintf(2, "Trace/BPT trap: %d\n", signal);
	else if (signal == SIGABRT)
		ft_dprintf(2, "Abort trap: %d\n", signal);
	else if (signal == SIGEMT)
		ft_dprintf(2, "EMT trap: %d\n", signal);
	else
		print_sig_message_2(signal);
}

int				ret_status(int ret, pid_t process, t_job *job)
{
	int		err;

	err = 0;
	if (WIFEXITED(ret))
		err = WEXITSTATUS(ret);
	else if (WIFSIGNALED(ret))
	{
		err = WTERMSIG(ret) + 128;
		print_sig_message(ret);
	}
	if (!job)
		job = get_job_pid(process);
	job->state = DONE;
	if (WIFSTOPPED(ret))
	{
		err = WSTOPSIG(ret) + 128;
		job->state = STOPPED_PENDING;
		job->status = ret;
	}
	return (err);
}
