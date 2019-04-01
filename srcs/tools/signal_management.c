#include <signal.h>
#include "shell.h"
#include "job.h"
#include "builtins.h"
#include "cmdline.h"

void	handler_signals(int sig)
{
	close(STDIN_FILENO);
	if (sig == SIGHUP)
		terminate_all_jobs();
}

void	handler_sigint(int sig)
{
	t_list	*tmp;
	t_list	*prev;
	t_job	*job;

	(void)sig;
	prev = 0;
	tmp = g_jobs;
	while (tmp)
	{
		job = tmp->content;
		if (job->state == RUNNING_FG)
			break ;
		if (job->pipe)
		{
			prev = tmp;
			tmp = job->pipe;
		}
		else
			tmp = tmp->next;
		if (!tmp && prev)
			tmp = prev->next;
	}
	if (!tmp)
		handle_end_of_text(g_cmdline);
}

void	sig_reset(void)
{
	struct sigaction	act;
	sigset_t			mask;
	int					x;

	x = 0;
	act.sa_flags = 0;
	act.sa_handler = SIG_DFL;
	while (x < 31)
		sigaction(x++, &act, 0);
	sigfillset(&mask);
	sigprocmask(SIG_UNBLOCK, &mask, 0);
}

void	set_sigmask(int type)
{
	sigset_t			mask;

	sigfillset(&mask);
	sigprocmask(type, &mask, 0);
}

void	set_signals_handlers_for_read(void)
{
	struct sigaction	act;
	int					x;

	sigfillset(&act.sa_mask);
	act.sa_handler = handler_signals;
	act.sa_flags = SA_RESTART;
	x = 1;
	while (x < 32)
	{
		sigdelset(&act.sa_mask, x);
		sigaction(x, &act, 0);
		sigaddset(&act.sa_mask, x++);
	}
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);
	act.sa_handler = handler_sigint;
	sigdelset(&act.sa_mask, SIGINT);
	sigaction(SIGINT, &act, 0);
	set_sigmask(SIG_UNBLOCK);
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
