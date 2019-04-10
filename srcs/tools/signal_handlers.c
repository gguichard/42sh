/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 15:33:00 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/10 14:30:31 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include "shell.h"
#include "cmdline.h"
#include "signals.h"
#include "job.h"
#include "builtins.h"

static void	handler_sigterm(void)
{
	if (check_stopped_job() && !g_cmdline->alloc->exit_rdy)
	{
		ft_dprintf(STDERR_FILENO, "exit\nThere are stopped jobs.\n");
		print_prompt_and_cmdline(g_cmdline);
		g_cmdline->alloc->exit_rdy = 1;
		return ;
	}
	g_sig = 15;
	close(STDIN_FILENO);
}

static void	handler_signals(int sig)
{
	if (sig == SIGWINCH)
		return (handle_sigwinch(sig));
	else if (sig == SIGTERM)
		return (handler_sigterm());
	else if (sig == SIGINT)
	{
		handle_end_of_text(g_cmdline);
		reset_cmdline(g_cmdline, g_cmdline->prompt.str
				, g_cmdline->prompt.offset);
		g_cmdline->alloc->ret_val = 128 + sig;
		return ;
	}
	g_sig = sig;
	close(STDIN_FILENO);
}

void		set_sigread(int is_interactive, int read, t_cmdline *cmdline)
{
	struct sigaction	act;

	if (!is_interactive || SIGNALS_ON == 0)
		return ;
	sigfillset(&act.sa_mask);
	if (read)
	{
		act.sa_flags = SA_RESTART;
		act.sa_handler = handler_signals;
		update_winsize(cmdline);
	}
	else
	{
		act.sa_handler = SIG_IGN;
		act.sa_flags = 0;
	}
	sigaction(SIGWINCH, &act, 0);
	sigaction(SIGINT, &act, 0);
	sigaction(SIGTERM, &act, 0);
}

void		set_sig_handlers(int is_interactive)
{
	struct sigaction	act;

	if (!is_interactive || SIGNALS_ON == 0)
		return ;
	sigfillset(&act.sa_mask);
	act.sa_handler = handler_signals;
	act.sa_flags = 0;
	action_sigs(&act);
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, 0);
	sigaction(SIGTSTP, &act, 0);
	sigaction(SIGTTOU, &act, 0);
	sigaction(SIGTTIN, &act, 0);
	set_sigmask(SIG_UNBLOCK);
}

void		print_nl_sigint(t_list *tmp)
{
	t_job	*job;
	int		print;

	print = 0;
	if (!check_job_state(tmp, STOPPED_PENDING))
	{
		job = check_job_state(tmp, SIG);
		if (job && job->status != SIGINT)
			return ;
		job = tmp->content;
		if (job->state == SIG && job->status == SIGINT)
			print = 1;
		tmp = job->pipe;
		while (tmp)
		{
			job = tmp->content;
			if (job->state == SIG && job->status == SIGINT)
				print = 1;
			tmp = tmp->next;
		}
	}
	if (print)
		write(STDOUT_FILENO, "\n", 1);
}
