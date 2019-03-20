/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 11:35:45 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/20 13:06:31 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "cmdline.h"

int	handle_line_start(t_cmdline *cmdline)
{
	int	offset;

	cmdline->saved_col = 0;
	if ((cmdline->row != 0 || cmdline->cursor.x > cmdline->prompt.offset)
			&& cmdline->cursor.x > 0)
	{
		if (cmdline->row == 0)
		{
			cmdline->input.offset = 0;
			cmdline->cursor.x = cmdline->prompt.offset;
		}
		else
		{
			offset = cmdline->input.offset;
			if (cmdline->cursor.y == 0)
				offset += cmdline->prompt.big_offset;
			cmdline->input.offset = offset - cmdline->cursor.x;
			cmdline->cursor.x = offset;
		}
		go_to_cursor_pos(cmdline->cursor);
	}
	return (1);
}

int	handle_line_end(t_cmdline *cmdline)
{
	int	col;
	int	offset;

	cmdline->saved_col = INT_MAX;
	col = cmdline->cursor.x;
	offset = cmdline->input.offset;
	while (offset < cmdline->input.size
			&& cmdline->input.buffer[offset] != '\n'
			&& (col + 1) < cmdline->winsize.ws_col)
	{
		col++;
		offset++;
	}
	cmdline->input.offset = offset;
	cmdline->cursor.x = col;
	go_to_cursor_pos(cmdline->cursor);
	return (1);
}
