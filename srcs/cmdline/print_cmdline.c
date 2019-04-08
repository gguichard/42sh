/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cmdline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/17 00:13:25 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/06 16:15:05 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <term.h>
#include "cmdline.h"

int			get_prompt_offset(t_cmdline *cmdline)
{
	size_t	total;

	total = cmdline->prompt.offset;
	if (cmdline->visual.toggle)
		total += ft_strlen(VISUAL_STRING);
	total %= ft_max(cmdline->winsize.ws_col, 1);
	return ((int)total);
}

void		update_cmdline_at_offset(t_cmdline *cmdline, char caller
		, int is_deletion)
{
	static char	*ce_tcap = NULL;
	t_cursor	saved_cursor;

	if (ce_tcap == NULL)
		ce_tcap = tgetstr("ce", NULL);
	if (cmdline->input.offset != cmdline->input.size)
	{
		saved_cursor = cmdline->cursor;
		print_line_by_line(cmdline
				, cmdline->input.buffer + cmdline->input.offset
				, cmdline->input.size - cmdline->input.offset, 1);
		if (is_deletion && (caller == '\n' || cmdline->cursor.x == 0))
		{
			if (caller == '\n')
			{
				write(STDOUT_FILENO, "\n", 1);
				cmdline->cursor.x = 0;
				cmdline->cursor.y += 1;
			}
			tputs(ce_tcap, 1, t_putchar);
		}
		go_to_cursor_pos(cmdline, saved_cursor);
	}
}

void		print_prompt_and_cmdline(t_cmdline *cmdline)
{
	t_cursor	start_cursor;
	size_t		offset;

	if (cmdline->visual.toggle)
		write(STDOUT_FILENO, VISUAL_STRING, ft_strlen(VISUAL_STRING));
	write(STDOUT_FILENO, cmdline->prompt.str, ft_strlen(cmdline->prompt.str));
	cmdline->cursor.x = get_prompt_offset(cmdline);
	cmdline->cursor.y = 0;
	start_cursor = cmdline->cursor;
	print_line_by_line(cmdline, cmdline->input.buffer, cmdline->input.size, 1);
	offset = 0;
	while (offset < cmdline->input.offset)
	{
		if (cmdline->input.buffer[offset] != '\n'
				&& (start_cursor.x + 1) < cmdline->winsize.ws_col)
			start_cursor.x += 1;
		else
		{
			start_cursor.x = 0;
			start_cursor.y += 1;
		}
		offset++;
	}
	go_to_cursor_pos(cmdline, start_cursor);
}
