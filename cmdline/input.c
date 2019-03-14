/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 16:28:07 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/14 23:17:57 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include <term.h>
#include "cmdline.h"

void		add_char_to_input(struct s_input *input, char c)
{
	int		new_cap;
	char	*new_buffer;

	if (input->size == input->capacity)
	{
		new_cap = input->capacity + INPUT_SIZE_INCR;
		new_buffer = (char *)malloc((new_cap + 1) * sizeof(char));
		if (new_buffer == NULL)
			return ;
		if (input->buffer != NULL)
			ft_memcpy(new_buffer, input->buffer, input->size);
		new_buffer[input->size] = '\0';
		input->capacity = new_cap;
		input->buffer = new_buffer;
	}
	if (input->offset != input->size)
		ft_memmove(input->buffer + input->offset + 1
				, input->buffer + input->offset
				, input->size - input->offset + 1);
	input->buffer[input->offset] = c;
	input->offset += 1;
	input->size += 1;
}

void		set_end_pos(t_cmdline *cmdline, t_cursor *cursor)
{
	int	offset;

	cursor->x = cmdline->prompt.offset;
	cursor->y = cmdline->cursor.y - cmdline->row;
	offset = 0;
	while (offset < cmdline->input.size)
	{
		if ((cursor->x + 1) < cmdline->winsize.ws_col
				&& cmdline->input.buffer[offset] != '\n')
			cursor->x += 1;
		else
		{
			cursor->x = 0;
			cursor->y += 1;
		}
		offset++;
	}
}

void		update_cmdline_after_offset(t_cmdline *cmdline)
{
	t_cursor	end_cursor;

	tputs(tgetstr("ce", NULL), 1, t_putchar);
	if ((cmdline->cursor.y + 1) < cmdline->winsize.ws_row)
	{
		tputs(tgetstr("cr", NULL), 1, t_putchar);
		tputs(tgetstr("do", NULL), 1, t_putchar);
		tputs(tgetstr("cd", NULL), 1, t_putchar);
		go_to_cursor_pos(cmdline->cursor);
	}
	write(STDOUT_FILENO
			, cmdline->input.buffer + cmdline->input.offset
			, cmdline->input.size - cmdline->input.offset);
	if ((cmdline->cursor.y + 1) == cmdline->winsize.ws_row)
	{
		set_end_pos(cmdline, &end_cursor);
		if (end_cursor.x == 1)
			cmdline->cursor.y -= 1;
	}
	go_to_cursor_pos(cmdline->cursor);
}

void		write_char_in_cmdline(t_cmdline *cmdline, char c)
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
	if (cmdline->input.offset != cmdline->input.size)
		update_cmdline_after_offset(cmdline);
}

void		read_input(t_cmdline *cmdline)
{
	char		c;
	const char	*seq;

	while (read(STDIN_FILENO, &c, 1) && c != 4)
	{
		if ((seq = get_sequence(cmdline, c)) != NULL)
			handle_sequence_char(cmdline, seq, c);
		else if (ft_isprint(c) && !cmdline->visual.toggle)
		{
			cmdline->saved_col = -1;
			add_char_to_input(&cmdline->input, c);
			write_char_in_cmdline(cmdline, c);
		}
	}
}
