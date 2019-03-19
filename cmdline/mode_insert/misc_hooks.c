/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 20:23:18 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/19 09:59:54 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <term.h>
#include "cmdline.h"

int	handle_clear(t_cmdline *cmdline)
{
	int	offset;

	cmdline->prompt.offset = ft_strlen(cmdline->prompt.str);
	cmdline->cursor.x = cmdline->prompt.offset;
	cmdline->cursor.y = 0;
	cmdline->row = 0;
	offset = 0;
	while (offset < cmdline->input.offset)
	{
		if (cmdline->input.buffer[offset] != '\n'
				&& (cmdline->cursor.x + 1) < cmdline->winsize.ws_col)
			cmdline->cursor.x += 1;
		else
		{
			cmdline->cursor.x = 0;
			cmdline->cursor.y += 1;
			cmdline->row += 1;
		}
		offset++;
	}
	tputs(tgetstr("cl", NULL), 1, t_putchar);
	write(STDOUT_FILENO, cmdline->prompt.str, cmdline->prompt.offset);
	print_mbstr(cmdline->input.buffer, cmdline->input.size);
	go_to_cursor_pos(cmdline->cursor);
	return (1);
}

int	handle_bell(t_cmdline *cmdline)
{
	(void)cmdline;
	tputs(tgetstr("bl", NULL), 1, t_putchar);
	return (1);
}
