#include "shell.h"
#include "job.h"

int		ret_status(int ret, pid_t process, t_job *job)
{
	int		err;

	err = 0;
	if (WIFEXITED(ret))
		err = WEXITSTATUS(ret);
	if (!job)
	{
		if (!(job = get_job_pid(process)))
			return (1);
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
