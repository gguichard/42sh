/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resize.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/15 15:20:55 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/18 11:01:59 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmdline.h"

extern t_cmdline	*g_cmdline;

static void	refresh_winsize(t_cmdline *cmdline)
{
	int	row;
	int	col;
	int	offset;

	set_cursor_pos(&cmdline->cursor);
	update_winsize(cmdline);
	row = 0;
	col = cmdline->prompt.offset;
	offset = 0;
	while (offset < cmdline->input.offset)
	{
		if ((col + 1) < cmdline->winsize.ws_col
				&& cmdline->input.buffer[offset] != '\n')
			col++;
		else
		{
			col = 0;
			row++;
		}
		offset++;
	}
	cmdline->row = row;
}

void		handle_sigwinch(int sig)
{
	(void)sig;
	refresh_winsize(g_cmdline);
}
