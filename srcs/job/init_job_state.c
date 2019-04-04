#include <sys/wait.h>
#include <unistd.h>
#include "shell.h"
#include "execution.h"
#include "job.h"

int		ret_status(int ret, pid_t process, t_job *job, t_exec_opt *opt)
{
	if (!job && WIFSIGNALED(ret) && WTERMSIG(ret) == SIGINT)
		write(1, "\n", 1);
	if (!job && !(job = get_job_pid(process)))
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
