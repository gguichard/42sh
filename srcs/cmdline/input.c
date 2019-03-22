/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 16:28:07 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/22 18:18:21 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <term.h>
#include "cmdline.h"

static void		write_char_in_cmdline(t_cmdline *cmdline, char c)
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
	update_cmdline_at_offset(cmdline);
}

static int		expand_buffer_capacity(t_cmdline *cmdline)
{
	int		new_cap;
	char	*new_buffer;

	if (cmdline->input.capacity >= INPUT_MAX_CAPACITY)
		return (0);
	new_cap = cmdline->input.capacity + INPUT_SIZE_INCR;
	new_buffer = (char *)malloc((new_cap + 1) * sizeof(char));
	if (new_buffer == NULL)
		return (0);
	if (cmdline->input.buffer != NULL)
	{
		ft_memcpy(new_buffer, cmdline->input.buffer, cmdline->input.size);
		free(cmdline->input.buffer);
	}
	new_buffer[cmdline->input.size] = L'\0';
	cmdline->input.capacity = new_cap;
	cmdline->input.buffer = new_buffer;
	return (1);
}

void			add_char_to_input(t_cmdline *cmdline, char c)
{
	if (cmdline->input.size == cmdline->input.capacity)
	{
		if (!expand_buffer_capacity(cmdline))
			return ;
	}
	ft_memmove(cmdline->input.buffer + cmdline->input.offset + 1
			, cmdline->input.buffer + cmdline->input.offset
			, (cmdline->input.size - cmdline->input.offset + 1));
	cmdline->input.buffer[cmdline->input.offset] = c;
	cmdline->input.offset += 1;
	cmdline->input.size += 1;
	write_char_in_cmdline(cmdline, c);
}

static void		process_input(t_cmdline *cmdline, char input)
{
	const t_seq	*seq;

	if ((seq = get_sequence(cmdline, input)) != NULL)
		handle_sequence(cmdline, seq);
	else if (!cmdline->visual.toggle)
	{
		cmdline->saved_col = -1;
		if (ft_isprint(input))
			add_char_to_input(cmdline, input);
	}
}

int				read_input(t_cmdline *cmdline, const char *prompt)
{
	char	input;

	write(STDOUT_FILENO, prompt, ft_strlen(prompt));
	set_cursor_pos(cmdline);
	cmdline->saved_col = -1;
	cmdline->prompt.str = prompt;
	cmdline->prompt.offset = cmdline->cursor.x;
	ft_memset(cmdline->input.buffer, 0, cmdline->input.capacity);
	cmdline->input.offset = 0;
	cmdline->input.size = 0;
	cmdline->input.reading = 1;
	while (cmdline->input.reading == 1
			&& read(STDIN_FILENO, &input, 1) == 1)
		process_input(cmdline, input);
	return (cmdline->input.reading != -1);
}
