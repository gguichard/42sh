/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_term.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:08:25 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/21 10:16:55 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <termios.h>
#include <unistd.h>
#include "libft.h"
#include "cmdline.h"

int	setup_term(t_cmdline *cmdline)
{
	struct termios	term;

	if (tcgetattr(STDIN_FILENO, &cmdline->default_term) == -1)
		return (0);
	ft_memcpy(&term, &cmdline->default_term, sizeof(struct termios));
	term.c_lflag &= ~(ICANON | ECHO | ECHONL);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
		return (0);
	return (1);
}

int	reset_term(t_cmdline *cmdline)
{
	if (tcsetattr(STDIN_FILENO, TCSANOW, &cmdline->default_term) == -1)
		return (0);
	return (1);
}

int	update_winsize(t_cmdline *cmdline)
{
	if (ioctl(0, TIOCGWINSZ, &cmdline->winsize) == -1)
		return (0);
	return (1);
}
