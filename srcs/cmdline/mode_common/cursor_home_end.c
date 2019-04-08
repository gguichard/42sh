/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_home_end.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 21:43:51 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/05 23:36:52 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term.h>
#include <limits.h>
#include "cmdline.h"

int	handle_home_key(t_cmdline *cmdline)
{
	cmdline->saved_col = 0;
	cmdline->input.offset = 0;
	go_to_cursor_pos(cmdline, (t_cursor){get_prompt_offset(cmdline), 0});
	return (1);
}

int	handle_end_key(t_cmdline *cmdline)
{
	int	x;
	int	y;

	x = cmdline->cursor.x;
	y = cmdline->cursor.y;
	while (cmdline->input.offset < cmdline->input.size)
	{
		if (cmdline->input.buffer[cmdline->input.offset] != '\n'
				&& (x + 1) < cmdline->winsize.ws_col)
			x++;
		else
		{
			x = 0;
			y++;
		}
		cmdline->input.offset += 1;
	}
	cmdline->saved_col = INT_MAX;
	go_to_cursor_pos(cmdline, (t_cursor){x, y});
	return (1);
}
