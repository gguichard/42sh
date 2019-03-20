/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_updown.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 12:39:11 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/20 10:53:02 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "cmdline.h"

int			handle_cursor_up(t_cmdline *cmdline)
{
	int	offset;
	int	rightest_col;
	int	col;

	if (cmdline->saved_col == -1)
		cmdline->saved_col = cmdline->cursor.x;
	if (cmdline->row == 0 || cmdline->cursor.y == 0)
		return (0);
	offset = cmdline->input.offset - cmdline->cursor.x - 1;
	rightest_col = get_rightmost_column(cmdline, offset);
	col = ft_min(rightest_col, cmdline->saved_col);
	cmdline->row -= 1;
	if ((cmdline->cursor.y - 1) == 0 && cmdline->row != 0)
		col = ft_max(cmdline->prompt.big_offset, col);
	if (cmdline->row != 0 || col > cmdline->prompt.offset)
		offset = offset - (rightest_col - col);
	else
	{
		col = cmdline->prompt.offset;
		offset = 0;
	}
	cmdline->input.offset = offset;
	cmdline->cursor = (t_cursor){col, cmdline->cursor.y - 1};
	go_to_cursor_pos(cmdline->cursor);
	return (1);
}

static int	cursor_down_offset(t_cmdline *cmdline, int row, int col)
{
	int	offset;

	offset = cmdline->input.offset;
	while (offset < cmdline->input.size
			&& (row != (cmdline->row + 1) || col < cmdline->saved_col))
	{
		if (cmdline->input.buffer[offset] != '\n'
				&& (col + 1) < cmdline->winsize.ws_col)
			col++;
		else
		{
			if (row != cmdline->row)
				break ;
			col = 0;
			row++;
		}
		offset++;
	}
	if (row == cmdline->row)
		return (0);
	cmdline->input.offset = offset;
	cmdline->cursor.x = col;
	return (1);
}

int			handle_cursor_down(t_cmdline *cmdline)
{
	if ((cmdline->cursor.y + 1) == cmdline->winsize.ws_row)
		return (0);
	if (cmdline->saved_col == -1)
		cmdline->saved_col = cmdline->cursor.x;
	if (!cursor_down_offset(cmdline, cmdline->row, cmdline->cursor.x))
		return (0);
	cmdline->row += 1;
	cmdline->cursor.y += 1;
	go_to_cursor_pos(cmdline->cursor);
	return (1);
}
