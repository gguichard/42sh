#include <signal.h>
#include "shell.h"
#include "job.h"
#include "builtins.h"
#include "cmdline.h"

void	sig_int(int sig)
{
	(void)sig;
	handle_end_of_text(g_cmdline);
}

void	sig_block_ign(void)
{
	struct sigaction	act_sigint;
	sigset_t			mask;

	act_sigint.sa_handler = sig_int;
	sigemptyset(&act_sigint.sa_mask);
	sigaddset(&act_sigint.sa_mask, SIGINT);
	sigaction(SIGINT, &act_sigint, 0);
	sigfillset(&mask);
	sigdelset(&mask, SIGINT);
	sigdelset(&mask, SIGTSTP);
	sigdelset(&mask, SIGQUIT);
	sigprocmask(SIG_BLOCK, &mask, 0);
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
			if (x == 15)
				write(1, "exit\n", 5);
			exit(128 + x);
		}
		x += 1;
	}
}
