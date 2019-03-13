/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_updown.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 12:39:11 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/13 22:23:38 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "cmdline.h"

int	handle_cursor_up(t_cmdline *cmdline)
{
	int	idx;
	int	row;
	int	rightest_col;
	int	col;

	if (cmdline->saved_col == -1)
		cmdline->saved_col = cmdline->cursor.x;
	if (cmdline->row == 0
			|| (idx = cmdline->input.offset - cmdline->cursor.x) <= 0)
		return (0);
	else
	{
		rightest_col = 1;
		if (cmdline->row == 1)
			rightest_col += cmdline->prompt.offset;
		while (idx > 0 && rightest_col < cmdline->winsize.ws_col
				&& cmdline->input.buffer[idx] != '\n')
		{
			idx--;
			rightest_col++;
		}
		col = cmdline->saved_col;
		if (col > rightest_col)
			col = rightest_col;
		else if (cmdline->row == 1 && col <= cmdline->prompt.offset)
			col = cmdline->prompt.offset + 1;
		idx = cmdline->input.offset - cmdline->cursor.x - (rightest_col - col);
	}
	cmdline->input.offset = idx;
	cmdline->row -= 1;
	cmdline->cursor.x = col;
	cmdline->cursor.y -= 1;
	go_to_cursor_pos(cmdline->cursor);
	return (1);
}

int	handle_cursor_down(t_cmdline *cmdline)
{
	int	idx;
	int	row;
	int	col;

	if (cmdline->saved_col == -1)
		cmdline->saved_col = cmdline->cursor.x;
	idx = cmdline->input.offset;
	row = cmdline->row;
	col = cmdline->cursor.x;
	while (idx < cmdline->input.size
			&& (row != cmdline->row + 1 || col < cmdline->saved_col))
	{
		if (cmdline->input.buffer[idx] != '\n' && col < cmdline->winsize.ws_col)
			col++;
		else
		{
			col = 1;
			row++;
		}
		idx++;
	}
	if (row == cmdline->row)
		return (0);
	cmdline->input.offset = idx;
	cmdline->row += 1;
	cmdline->cursor.x = col;
	cmdline->cursor.y += 1;
	go_to_cursor_pos(cmdline->cursor);
	return (1);
}
