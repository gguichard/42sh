#include <signal.h>
#include "shell.h"
#include "job.h"
#include "builtins.h"
#include "cmdline.h"

void	sig_int(int sig)
{
	(void)sig;
	ft_printf("Sigint handler\n");
	handle_end_of_text(g_cmdline);
}

void	sig_handler(int sig, siginfo_t *info, void *uap)
{
	t_job	*job;

	(void)uap;
	job = get_job_allpid(info->si_pid);
	ft_dprintf(2 ,"pid %d status %d code %d job found %d\n", info->si_pid, info->si_status, info->si_code, job != 0);
	if (sig == SIGCHLD)
	{
		// ft_printf("pid %d status %d code %d job found %d\n", info->si_pid, info->si_status, info->si_code, job != 0);
		// if (info->si_code == CLD_EXITED)
		// {
		// 	if (job->state != RUNNING_FG)
		// 	{
		// 		job->state = DONE;
		// 		job->status = info->si_status;
		// 		print_job(info->si_pid, 0);
		// 	}
		// 	job->state = DONE;
		// 	job->status = info->si_status;
		// }
		if (info->si_code == CLD_STOPPED && job->state != STOPPED)
			update_job_state(info->si_pid, STOPPED_PENDING);
		else if (info->si_code == CLD_CONTINUED)
			update_job_state(info->si_pid, RUNNING_BG);
	}
}

void	sig_reset(void)
{
	struct sigaction	act;
	sigset_t			mask;

	act.sa_handler = SIG_DFL;
	sigaction(SIGINT, &act, 0);
	sigaction(SIGQUIT, &act, 0);
	sigaction(SIGTSTP, &act, 0);
	sigfillset(&mask);
	sigdelset(&mask, SIGINT);
	sigdelset(&mask, SIGTSTP);
	sigdelset(&mask, SIGQUIT);
	sigprocmask(SIG_UNBLOCK, &mask, 0);
}

void	sig_unblock_sig(int sig)
{
	sigset_t	mask;

	sigemptyset(&mask);
	sigdelset(&mask, sig);
	sigprocmask(SIG_SETMASK, &mask, 0);
}

void	sig_block_sig(int sig)
{
	sigset_t	mask;

	sigemptyset(&mask);
	sigaddset(&mask, sig);
	sigprocmask(SIG_SETMASK, &mask, 0);
}

void	sig_block_ign(void)
{
	struct sigaction	act;
	// sigset_t			mask;

	act.sa_sigaction = sig_handler;
	act.sa_flags = SA_RESTART | SA_SIGINFO;
	sigaction(SIGCHLD, &act, 0);
	// act.sa_handler = sig_int;
	// act.sa_flags = SA_RESTART;
	// sigemptyset(&act.sa_mask);
	// sigaddset(&act.sa_mask, SIGINT);
	// sigaction(SIGINT, &act, 0);
	// sigemptyset(&act.sa_mask);
	// act.sa_handler = SIG_IGN;
	// act.sa_flags = 0;
	// sigaction(SIGQUIT, &act, 0);
	// sigaction(SIGTSTP, &act, 0);
	// sigdelset(&mask, SIGINT);
	// sigdelset(&mask, SIGTSTP);
	// sigdelset(&mask, SIGQUIT);
	// sigprocmask(SIG_BLOCK, &mask, 0);
}

void	sigs_wait_line(t_alloc *alloc)
{
	sigset_t	mask;
	int			x;

	x = 1;
	sigemptyset(&mask);
	sigpending(&mask);
	while (x < 32)
	{
		if (sigismember(&mask, x))
		{
			terminate_all_jobs();
			save_history_entries(alloc, &alloc->cmdline.history);
			reset_term(&alloc->cmdline);
			del_alloc(alloc);
			sig_reset();
			if (x == 15)
				write(1, "exit\n", 5);
			exit(128 + x);
		}
		x += 1;
	}
}
