/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 10:11:35 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/19 10:53:19 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmdline.h"

int			get_rightmost_column(t_cmdline *cmdline, int offset)
{
	int	col;

	if (cmdline->input.buffer[offset] != '\n')
		return (cmdline->winsize.ws_col - 1);
	else
	{
		col = 0;
		while (offset > 0 && cmdline->input.buffer[offset - 1] != '\n')
		{
			col++;
			offset--;
		}
		if (offset == 0)
			col += cmdline->prompt.offset;
		if (cmdline->winsize.ws_col <= 0)
			return (0);
		else
			return (col % cmdline->winsize.ws_col);
	}
}

t_cursor	go_to_offset(t_cmdline *cmdline, int offset)
{
	t_cursor	cursor;
	int			idx;

	cursor.x = cmdline->prompt.offset;
	cursor.y = cmdline->cursor.y - cmdline->row;
	idx = 0;
	while (idx < offset)
	{
		if ((cursor.x + 1) < cmdline->winsize.ws_col
				&& cmdline->input.buffer[idx] != '\n')
			cursor.x += 1;
		else
		{
			cursor.x = 0;
			cursor.y += 1;
		}
		idx++;
	}
	go_to_cursor_pos(cursor);
	return (cursor);
}

void		recompute_cursor(t_cmdline *cmdline)
{
	int	offset;

	offset = 0;
	while (offset < cmdline->input.offset)
	{
		if (cmdline->input.buffer[offset] != '\n'
				&& (cmdline->cursor.x + 1) < cmdline->winsize.ws_col)
			cmdline->cursor.x += 1;
		else
		{
			cmdline->cursor.x = 0;
			cmdline->cursor.y += 1;
			cmdline->row += 1;
		}
		offset++;
	}
}