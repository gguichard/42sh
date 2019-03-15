/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 16:28:07 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/15 19:14:31 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <term.h>
#include "cmdline.h"

void		update_cmdline_after_offset(t_cmdline *cmdline)
{
	if (cmdline->input.offset != cmdline->input.size)
	{
		write(STDOUT_FILENO
				, cmdline->input.buffer + cmdline->input.offset
				, cmdline->input.size - cmdline->input.offset);
		go_to_cursor_pos(cmdline->cursor);
	}
}

static void	write_char_in_cmdline(t_cmdline *cmdline, char c)
{
	if (c == '\n')
		tputs(tgetstr("ce", NULL), 1, t_putchar);
	write(STDOUT_FILENO, &c, 1);
	if (c != '\n' && (cmdline->cursor.x + 1) < cmdline->winsize.ws_col)
		cmdline->cursor.x += 1;
	else
	{
		if (c != '\n')
		{
			tputs(tgetstr("cr", NULL), 1, t_putchar);
			tputs(tgetstr("do", NULL), 1, t_putchar);
		}
		cmdline->cursor.x = 0;
		if ((cmdline->cursor.y + 1) < cmdline->winsize.ws_row)
			cmdline->cursor.y += 1;
		cmdline->row += 1;
	}
	update_cmdline_after_offset(cmdline);
}

void		add_char_to_input(t_cmdline *cmdline, char c)
{
	int		new_cap;
	char	*new_buffer;

	if (cmdline->input.size == cmdline->input.capacity)
	{
		new_cap = cmdline->input.capacity + INPUT_SIZE_INCR;
		new_buffer = (char *)malloc((new_cap + 1) * sizeof(char));
		if (new_buffer == NULL)
			return ;
		if (cmdline->input.buffer != NULL)
			ft_memcpy(new_buffer, cmdline->input.buffer, cmdline->input.size);
		new_buffer[cmdline->input.size] = '\0';
		cmdline->input.capacity = new_cap;
		cmdline->input.buffer = new_buffer;
	}
	if (cmdline->input.offset != cmdline->input.size)
		ft_memmove(cmdline->input.buffer + cmdline->input.offset + 1
				, cmdline->input.buffer + cmdline->input.offset
				, cmdline->input.size - cmdline->input.offset + 1);
	cmdline->input.buffer[cmdline->input.offset] = c;
	cmdline->input.offset += 1;
	cmdline->input.size += 1;
	write_char_in_cmdline(cmdline, c);
}

void		read_input(t_cmdline *cmdline)
{
	char		c;
	const char	*seq;

	while (read(STDIN_FILENO, &c, 1) && c != 4)
	{
		if ((seq = get_sequence(cmdline, c)) != NULL)
			handle_sequence(cmdline, seq);
		else if (ft_isprint(c) && !cmdline->visual.toggle)
		{
			cmdline->saved_col = -1;
			add_char_to_input(cmdline, c);
		}
	}
}
