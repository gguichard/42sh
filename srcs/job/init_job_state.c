#include <sys/wait.h>
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
	}
	if (WIFEXITED(ret))
	{
		job->state = DONE;
		err = WEXITSTATUS(ret);
		job->status = WEXITSTATUS(ret);
	}
	else if (WIFSTOPPED(ret))
	{
		err = WSTOPSIG(ret) + 128;
		job->state = STOPPED_PENDING;
		job->status = WSTOPSIG(ret);
	}
	else if (WIFSIGNALED(ret))
	{
		err = WTERMSIG(ret) + 128;
		job->state = SIG;
		job->status = WTERMSIG(ret);
	}
	return (err);
}
