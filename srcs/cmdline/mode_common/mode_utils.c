/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 10:11:35 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/05 23:37:44 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "cmdline.h"

int		get_rightmost_column(t_cmdline *cmdline, int offset)
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
			col += get_prompt_offset(cmdline);
		if (cmdline->winsize.ws_col <= 0)
			return (0);
		else
			return (col % cmdline->winsize.ws_col);
	}
}

void	go_to_offset(t_cmdline *cmdline, int offset)
{
	t_cursor	cursor;
	int			idx;

	cursor.x = get_prompt_offset(cmdline);
	cursor.y = 0;
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
	go_to_cursor_pos(cmdline, cursor);
}
