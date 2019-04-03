#include <signal.h>
#include "shell.h"
#include "signals.h"
#include "job.h"
#include "builtins.h"

static void	handler_sigterm(void)
{
	t_list	*tmp;

	tmp = g_jobs;
	while (tmp)
	{
		if (check_job_state(tmp, RUNNING_FG))
			return ;
		tmp = tmp->next;
	}
	if (check_stopped_job() && !retry_exit_job(-1))
	{
		ft_dprintf(STDERR_FILENO, "exit\nThere are stopped jobs.\n");
		print_prompt_and_cmdline(g_cmdline);
		retry_exit_job(1);
		return ;
	}
	g_sig = 15;
	close(STDIN_FILENO);
}

static void	handler_signals(int sig)
{
	if (sig == SIGTERM)
		return (handler_sigterm());
	g_sig = sig;
	if (sig == SIGINT)
	{
		handle_end_of_text(g_cmdline);
		return ;
	}
	close(STDIN_FILENO);
	if (sig == SIGHUP)
		terminate_all_jobs(SIGHUP);
}

void		set_signals_handlers(void)
{
	struct sigaction	act;

	set_sigmask(SIG_UNBLOCK);
	sigfillset(&act.sa_mask);
	act.sa_handler = handler_signals;
	act.sa_flags = 0;
	action_sigs(&act);
	act.sa_flags = SA_RESTART;
	sigaction(SIGTERM, &act, 0);
	sigaction(SIGHUP, &act, 0);
	sigaction(SIGINT, &act, 0);
	act.sa_flags = 0;
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, 0);
	sigaction(SIGTSTP, &act, 0);
	sigaction(SIGTTOU, &act, 0);
	sigaction(SIGTTIN, &act, 0);
}
