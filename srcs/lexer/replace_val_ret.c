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

static char	*sig_str_2(int signal)
{
	if (signal == SIGFPE)
		return ("Floating point exception: 8");
	else if (signal == SIGKILL)
		return ("Killed: 9");
	else if (signal == SIGBUS)
		return ("Bus error: 10");
	else if (signal == SIGSEGV)
		return ("Segmentation fault: 11");
	else if (signal == SIGSYS)
		return ("Bad system call: 12");
	else if (signal == SIGALRM)
		return ("Alarm clock: 14");
	else if (signal == SIGTERM)
		return ("Terminated: 15");
	return ("Undefined Signal");
}

char	*sig_str(int status)
{
	int	signal;

	signal = WTERMSIG(status);
	if (signal == SIGHUP)
		return ("Hangup: 1");
	else if (signal == SIGQUIT)
		return ("Quit: 3");
	else if (signal == SIGILL)
		return ("Illegal instruction: 4");
	else if (signal == SIGTRAP)
		return ("Trace/BPT trap: 5");
	else if (signal == SIGABRT)
		return ("Abort trap: 6");
	else if (signal == SIGEMT)
		return ("EMT trap: 7");
	else
		return (sig_str_2(signal));
}

int				ret_status(int ret, pid_t process, t_job *job)
{
	int		err;

	err = 0;
	if (WIFEXITED(ret))
		err = WEXITSTATUS(ret);
	if (!job)
	{
		job = get_job_pid(process);
		job->status = ret;
	}
	job->state = DONE;
	if (WIFSTOPPED(ret))
	{
		err = WSTOPSIG(ret) + 128;
		job->state = STOPPED_PENDING;
		job->status = ret;
	}
	else if (WIFSIGNALED(ret))
	{
		err = WTERMSIG(ret) + 128;
		job->state = SIG;
	}
	return (err);
}
