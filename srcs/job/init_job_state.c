#include "shell.h"
#include "job.h"

int		ret_status(int ret, pid_t process, t_job *job)
{
	int		err;

	err = 0;
	if (!job)
	{
		if (!(job = get_job_pid(process)))
			return (1);
		job->status = ret;
	}
	if (WIFSTOPPED(ret))
	{
		err = WSTOPSIG(ret) + 128;
		job->state = STOPPED_PENDING;
		job->status = ret;
		ft_printf("coucou");
	}
	else if (WIFSIGNALED(ret))
	{
		err = WTERMSIG(ret) + 128;
		job->state = SIG;
	}
	else if (WIFEXITED(ret))
	{
		job->state = DONE;
		err = WEXITSTATUS(ret);
	}
	return (err);
}
