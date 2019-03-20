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

int				ret_status(int ret, pid_t process)
{
	t_job	*job;
	int		err;

	err = 0;
	if (WIFEXITED(ret))
	{
		err = WEXITSTATUS(ret);
		if (process)
		{
			job = get_job_pid(process);
			job->state = DONE;
		}
	}
	else if (WIFSIGNALED(ret))
		err = WTERMSIG(ret) + 128;
	else if (WIFSTOPPED(ret))
	{
		err = WSTOPSIG(ret) + 128;
		if (process)
		{
			job = get_job_pid(process);
			job->state = STOPPED_PENDING;
			job->status = ret;
		}
	}
	return (err);
}
