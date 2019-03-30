/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cmdline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/17 00:13:25 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/30 20:55:19 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include "cmdline.h"

void	update_cmdline_at_offset(t_cmdline *cmdline)
{
	t_cursor	saved_cursor;

	if (cmdline->input.offset != cmdline->input.size)
	{
		saved_cursor = cmdline->cursor;
		print_line_by_line(cmdline, cmdline->input.offset);
		go_to_cursor_pos(cmdline, saved_cursor);
	}
}

void	print_prompt_and_cmdline(t_cmdline *cmdline)
{
	size_t	total;

	total = 0;
	if (cmdline->visual.toggle)
		total += ft_max(write(STDOUT_FILENO, "(visual) ", 9), 0);
	total += ft_max(write(STDOUT_FILENO, cmdline->prompt.str
				, ft_strlen(cmdline->prompt.str)), 0);
	cmdline->cursor.x = total % ft_max(cmdline->winsize.ws_col, 1);
	cmdline->cursor.y = 0;
	cmdline->prompt.offset = cmdline->cursor.x;
	print_only_cmdline(cmdline);
}
