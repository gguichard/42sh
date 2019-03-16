/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 16:28:07 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/16 17:57:20 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <term.h>
#include "cmdline.h"

static void	print_line_by_line(t_cmdline *cmdline, t_cursor end_cursor)
{
	static const char	*ce_tcap = NULL;
	const char			*buffer;
	const char			*eol;
	int					buff_len;
	int					offset;

	if (ce_tcap == NULL)
		ce_tcap = tgetstr("ce", NULL);
	buffer = cmdline->input.buffer + cmdline->input.offset;
	buff_len = cmdline->input.size - cmdline->input.offset;
	while (buff_len > 0)
	{
		eol = ft_memchr(buffer, '\n', buff_len);
		offset = (eol == NULL) ? buff_len : (eol - buffer);
		end_cursor.x += offset;
		end_cursor.y += end_cursor.x / ft_max(1, cmdline->winsize.ws_col);
		end_cursor.x %= ft_max(1, cmdline->winsize.ws_col);
		write(STDOUT_FILENO, buffer, offset);
		tputs(ce_tcap, 1, t_putchar);
		if (eol != NULL)
		{
			write(STDOUT_FILENO, "\n", 1);
			end_cursor.x = 0;
			end_cursor.y += 1;
			offset++;
		}
		buffer += offset;
		buff_len -= offset;
	}
	if (end_cursor.y == cmdline->winsize.ws_row)
	{
		tputs(tgetstr("sf", NULL), 1, t_putchar);
		cmdline->cursor.y -= 1;
	}
}

void		update_cmdline_after_offset(t_cmdline *cmdline)
{
	if (cmdline->input.offset != cmdline->input.size)
	{
		print_line_by_line(cmdline, cmdline->cursor);
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
			write(STDOUT_FILENO, "\n", 1);
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
	char		input;
	const char	*seq;

	while (read(STDIN_FILENO, &input, 1) && input != 4)
	{
		if ((seq = get_sequence(cmdline, input)) != NULL)
			handle_sequence(cmdline, seq);
		else if (ft_isprint(input) && !cmdline->visual.toggle)
		{
			cmdline->saved_col = -1;
			add_char_to_input(cmdline, input);
		}
	}
}
