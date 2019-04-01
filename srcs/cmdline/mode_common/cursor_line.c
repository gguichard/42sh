/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 11:35:45 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/30 18:53:25 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "cmdline.h"

int	handle_line_start(t_cmdline *cmdline)
{
	int	x;

	cmdline->saved_col = 0;
	if ((cmdline->cursor.y != 0 || cmdline->cursor.x > cmdline->prompt.offset)
			&& cmdline->cursor.x > 0)
	{
		x = (cmdline->cursor.y == 0) ? cmdline->prompt.offset : 0;
		cmdline->input.offset -= cmdline->cursor.x - x;
		go_to_cursor_pos(cmdline, (t_cursor){x, cmdline->cursor.y});
	}
	return (1);
}

int	handle_line_end(t_cmdline *cmdline)
{
	int	x;
	int	offset;

	cmdline->saved_col = INT_MAX;
	x = cmdline->cursor.x;
	offset = cmdline->input.offset;
	while (offset < cmdline->input.size
			&& cmdline->input.buffer[offset] != '\n'
			&& (x + 1) < cmdline->winsize.ws_col)
	{
		x++;
		offset++;
	}
	cmdline->input.offset = offset;
	go_to_cursor_pos(cmdline, (t_cursor){x, cmdline->cursor.y});
	return (1);
}
