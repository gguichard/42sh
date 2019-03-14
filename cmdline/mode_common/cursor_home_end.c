/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_home_end.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 21:43:51 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/14 10:51:58 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "cmdline.h"

int	handle_home_key(t_cmdline *cmdline)
{
	cmdline->saved_col = INT_MIN;
	cmdline->input.offset = 0;
	cmdline->cursor.x = cmdline->prompt.offset + 1;
	cmdline->cursor.y -= cmdline->row;
	cmdline->row = 0;
	go_to_cursor_pos(cmdline->cursor);
	return (0);
}

int	handle_end_key(t_cmdline *cmdline)
{
	int	col;
	int	rows;

	rows = 0;
	col = cmdline->cursor.x;
	while (cmdline->input.offset < cmdline->input.size)
	{
		if (cmdline->input.buffer[cmdline->input.offset] != '\n'
				&& col != cmdline->winsize.ws_col)
			col++;
		else
		{
			col = 1;
			rows++;
		}
		cmdline->input.offset += 1;
	}
	cmdline->saved_col = INT_MAX;
	cmdline->cursor.x = col;
	cmdline->cursor.y += rows;
	cmdline->row += rows;
	go_to_cursor_pos(cmdline->cursor);
	return (0);
}
