/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_updown.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/13 12:39:11 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/05 23:43:03 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "cmdline.h"

int			handle_cursor_up(t_cmdline *cmdline)
{
	size_t	offset;
	int		rightest_col;
	int		col;

	if (cmdline->saved_col == -1)
		cmdline->saved_col = cmdline->cursor.x;
	if (cmdline->cursor.y == 0)
		return (0);
	offset = cmdline->input.offset - cmdline->cursor.x - 1;
	rightest_col = get_rightmost_column(cmdline, offset);
	col = ft_min(rightest_col, cmdline->saved_col);
	if ((cmdline->cursor.y - 1) != 0 || col > get_prompt_offset(cmdline))
		offset = offset - (rightest_col - col);
	else
	{
		col = get_prompt_offset(cmdline);
		offset = 0;
	}
	cmdline->input.offset = offset;
	go_to_cursor_pos(cmdline, (t_cursor){col, cmdline->cursor.y - 1});
	return (1);
}

static int	cursor_down_offset(t_cmdline *cmdline, int col)
{
	int		row;
	size_t	offset;

	row = cmdline->cursor.y;
	offset = cmdline->input.offset;
	while (offset < cmdline->input.size
			&& (row != (cmdline->cursor.y + 1) || col < cmdline->saved_col))
	{
		if (cmdline->input.buffer[offset] != '\n'
				&& (col + 1) < cmdline->winsize.ws_col)
			col++;
		else
		{
			if (row != cmdline->cursor.y)
				break ;
			col = 0;
			row++;
		}
		offset++;
	}
	if (row == cmdline->cursor.y)
		return (-1);
	cmdline->input.offset = offset;
	return (col);
}

int			handle_cursor_down(t_cmdline *cmdline)
{
	int	ret;

	if (cmdline->saved_col == -1)
		cmdline->saved_col = cmdline->cursor.x;
	if ((ret = cursor_down_offset(cmdline, cmdline->cursor.x)) == -1)
		return (0);
	go_to_cursor_pos(cmdline, (t_cursor){ret, cmdline->cursor.y + 1});
	return (1);
}
