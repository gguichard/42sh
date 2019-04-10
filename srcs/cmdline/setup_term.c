/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_term.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:08:25 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/09 22:21:15 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <term.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include "cmdline.h"
#include "signals.h"

int	setup_term(t_cmdline *cmdline)
{
	struct termios		term;
	struct sigaction	act;

	set_signals_handlers(cmdline->alloc->is_interactive, 1);
	if (cmdline->alloc->is_interactive)
	{
		update_winsize(cmdline);
		act.sa_handler = handle_sigwinch;
		act.sa_flags = SA_RESTART;
		sigfillset(&act.sa_mask);
		sigaction(SIGWINCH, &act, 0);
	}
	if (!cmdline->term_init
			&& tcgetattr(STDIN_FILENO, &cmdline->default_term) == -1)
		return (0);
	cmdline->term_init = 1;
	ft_memcpy(&term, &cmdline->default_term, sizeof(struct termios));
	term.c_lflag &= ~(ICANON | ECHO | ECHONL | ISIG);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		return (0);
	return (1);
}

int	reset_term(t_cmdline *cmdline)
{
	struct sigaction	act;

	if (cmdline->alloc->is_interactive && SIGNALS_ON)
	{
		act.sa_handler = SIG_IGN;
		act.sa_flags = 0;
		sigaction(SIGWINCH, &act, 0);
		set_sigmask(SIG_BLOCK);
	}
	if (tcsetattr(STDIN_FILENO, TCSANOW, &cmdline->default_term) == -1)
		return (0);
	return (1);
}

int	update_winsize(t_cmdline *cmdline)
{
	int	ret;

	ret = 1;
	if (ioctl(STDIN_FILENO, TIOCGWINSZ, &cmdline->winsize) == -1)
		ret = 0;
	if (!ret || cmdline->winsize.ws_col == 0)
		cmdline->winsize.ws_col = tgetnum("co");
	if (!ret || cmdline->winsize.ws_row == 0)
		cmdline->winsize.ws_row = tgetnum("li");
	return (ret);
}
