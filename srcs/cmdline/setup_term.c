/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_term.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:08:25 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/10 12:37:05 by jocohen          ###   ########.fr       */
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

	set_sig_handlers(cmdline->alloc->is_interactive);
	set_sigread(cmdline->alloc->is_interactive, 1, cmdline);
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
	set_sigread(cmdline->alloc->is_interactive, 0, 0);
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
