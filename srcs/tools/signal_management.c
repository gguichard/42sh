#include "shell.h"
#include "builtins.h"
#include "job.h"

void		set_sigmask(int type)
{
	sigset_t			mask;

	sigfillset(&mask);
	sigprocmask(type, &mask, 0);
}

void	action_sigs(struct sigaction *act)
{
	sigaction(SIGILL, act, 0);
	sigaction(SIGTRAP, act, 0);
	sigaction(SIGABRT, act, 0);
	sigaction(SIGEMT, act, 0);
	sigaction(SIGFPE, act, 0);
	sigaction(SIGBUS, act, 0);
	sigaction(SIGSEGV, act, 0);
	sigaction(SIGSYS, act, 0);
	sigaction(SIGPIPE, act, 0);
	sigaction(SIGALRM, act, 0);
	sigaction(SIGXCPU, act, 0);
	sigaction(SIGXFSZ, act, 0);
	sigaction(SIGVTALRM, act, 0);
	sigaction(SIGPROF, act, 0);
	sigaction(SIGUSR1, act, 0);
	sigaction(SIGUSR2, act, 0);
}

void		sig_reset(void)
{
	struct sigaction	act;

	act.sa_flags = 0;
	act.sa_handler = SIG_DFL;
	sigaction(SIGHUP, &act, 0);
	sigaction(SIGINT, &act, 0);
	sigaction(SIGQUIT, &act, 0);
	sigaction(SIGTSTP, &act, 0);
	sigaction(SIGTTOU, &act, 0);
	sigaction(SIGTTIN, &act, 0);
	sigaction(SIGTERM, &act, 0);
	action_sigs(&act);
	set_sigmask(SIG_UNBLOCK);
}

void		sigs_wait_line(t_alloc *alloc)
{
	sigset_t	mask;
	int			x;

	x = 1;
	sigemptyset(&mask);
	sigpending(&mask);
	while (x < 32)
	{
		if (x == 2 || x == 3 || x == 15 || x == 16 || (x >= 18 && x <= 23)
			|| x == 28 || x == 29 || !sigismember(&mask, x))
			x += 1;
		else if (sigismember(&mask, x))
		{
			if (x == 1)
				terminate_all_jobs(SIGHUP);
			g_sig = x;
			builtin_exit(0, alloc);
		}
	}
}
