/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_moves.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 11:15:36 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/30 19:44:31 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <term.h>
#include "cmdline.h"

int	handle_move_left(t_cmdline *cmdline)
{
	static char	*le_tcap = NULL;
	t_cursor	new_cursor;

	if (le_tcap == NULL)
		le_tcap = tgetstr("le", NULL);
	cmdline->saved_col = -1;
	if (cmdline->input.offset == 0)
		return (0);
	cmdline->input.offset -= 1;
	if (cmdline->cursor.x > 0)
	{
		tputs(le_tcap, 1, t_putchar);
		cmdline->cursor.x -= 1;
	}
	else
	{
		new_cursor.x = get_rightmost_column(cmdline, cmdline->input.offset);
		new_cursor.y = cmdline->cursor.y - 1;
		go_to_cursor_pos(cmdline, new_cursor);
	}
	return (1);
}

int	handle_move_right(t_cmdline *cmdline)
{
	static char	*nd_tcap = NULL;

	if (nd_tcap == NULL)
		nd_tcap = tgetstr("nd", NULL);
	cmdline->saved_col = -1;
	if (cmdline->input.offset == cmdline->input.size)
		return (0);
	cmdline->cursor.x += 1;
	if (cmdline->cursor.x < cmdline->winsize.ws_col
			&& cmdline->input.buffer[cmdline->input.offset] != '\n')
		tputs(nd_tcap, 1, t_putchar);
	else
		go_to_cursor_pos(cmdline, (t_cursor){0, cmdline->cursor.y + 1});
	cmdline->input.offset += 1;
	return (1);
}
