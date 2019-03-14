/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 11:35:45 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/14 12:44:58 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "cmdline.h"

int	handle_line_start(t_cmdline *cmdline)
{
	cmdline->saved_col = INT_MIN;
	if (cmdline->cursor.x > 0 && (cmdline->row != 0
				|| cmdline->cursor.x > cmdline->prompt.offset))
	{
		if (cmdline->row == 0)
		{
			cmdline->input.offset = 0;
			cmdline->cursor.x = cmdline->prompt.offset;
		}
		else
		{
			cmdline->input.offset = cmdline->input.offset - cmdline->cursor.x;
			cmdline->cursor.x = 0;
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
