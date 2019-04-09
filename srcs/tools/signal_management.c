/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_management.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 15:33:17 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/09 11:22:15 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "shell.h"
#include "builtins.h"
#include "job.h"

void	set_sigmask(int type)
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

void	sig_set_all(void (*handler)(int))
{
	struct sigaction	act;

	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	act.sa_handler = handler;
	sigaction(SIGHUP, &act, 0);
	sigaction(SIGINT, &act, 0);
	sigaction(SIGQUIT, &act, 0);
	sigaction(SIGTSTP, &act, 0);
	sigaction(SIGTTOU, &act, 0);
	sigaction(SIGTTIN, &act, 0);
	sigaction(SIGTERM, &act, 0);
	action_sigs(&act);
}

void	sig_reset(void)
{
	sig_set_all(SIG_IGN);
	set_sigmask(SIG_UNBLOCK);
	sig_set_all(SIG_DFL);
}

void	sigs_wait_line(t_alloc *alloc)
{
	sigset_t	mask;
	int			x;

	x = 1;
	if (!alloc->is_interactive)
		return ;
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
