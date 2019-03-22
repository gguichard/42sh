/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 16:28:07 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/22 16:13:51 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <wchar.h>
#include <term.h>
#include "cmdline.h"

static void		write_char_in_cmdline(t_cmdline *cmdline, wint_t c)
{
	char	dest[4];

	if (c == '\n')
		tputs(tgetstr("ce", NULL), 1, t_putchar);
	write(STDOUT_FILENO, dest, utf8_encode(dest, c));
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
	wchar_t	*new_buffer;

	if (cmdline->input.capacity >= INPUT_MAX_CAPACITY)
		return (0);
	new_cap = cmdline->input.capacity + INPUT_SIZE_INCR;
	new_buffer = (wchar_t *)malloc((new_cap + 1) * sizeof(wint_t));
	if (new_buffer == NULL)
		return (0);
	if (cmdline->input.buffer != NULL)
	{
		ft_memcpy(new_buffer, cmdline->input.buffer
				, cmdline->input.size * sizeof(wint_t));
		free(cmdline->input.buffer);
	}
	new_buffer[cmdline->input.size] = L'\0';
	cmdline->input.capacity = new_cap;
	cmdline->input.buffer = new_buffer;
	return (1);
}

void			add_char_to_input(t_cmdline *cmdline, wint_t c)
{
	if (cmdline->input.size == cmdline->input.capacity)
	{
		if (!expand_buffer_capacity(cmdline))
			return ;
	}
	ft_memmove(cmdline->input.buffer + cmdline->input.offset + 1
			, cmdline->input.buffer + cmdline->input.offset
			, (cmdline->input.size - cmdline->input.offset + 1)
			* sizeof(wint_t));
	cmdline->input.buffer[cmdline->input.offset] = c;
	cmdline->input.offset += 1;
	cmdline->input.size += 1;
	write_char_in_cmdline(cmdline, c);
}

static wint_t	read_n_decode_utf8(unsigned char input)
{
	uint32_t		utf8;
	unsigned char	*buffer;
	int				idx;
	int				len;

	utf8 = 0;
	len = utf8_size(input);
	if (len == 1 && !ft_isprint(input))
		return (0);
	buffer = (unsigned char *)&utf8;
	buffer += len - 1;
	*buffer = input;
	if (len > 0)
	{
		idx = 0;
		while (idx < (len - 1))
		{
			if (read(STDIN_FILENO, --buffer, 1) != 1)
				return (0);
			idx++;
		}
	}
	return (utf8_decode(utf8, len));
}

int				read_input(t_cmdline *cmdline, const char *prompt)
{
	unsigned char	input;
	const t_seq		*seq;
	wint_t			point;

	write(STDOUT_FILENO, prompt, ft_strlen(prompt));
	set_cursor_pos(&cmdline->cursor);
	cmdline->saved_col = -1;
	cmdline->prompt.str = prompt;
	cmdline->prompt.offset = cmdline->cursor.x;
	ft_memset(cmdline->input.buffer, 0, cmdline->input.capacity);
	cmdline->input.offset = 0;
	cmdline->input.size = 0;
	cmdline->input.reading = 1;
	while (cmdline->input.reading == 1
			&& read(STDIN_FILENO, &input, sizeof(char)) == 1)
	{
		if ((seq = get_sequence(cmdline, input)) != NULL)
			handle_sequence(cmdline, seq);
		else if (!cmdline->visual.toggle)
		{
			cmdline->saved_col = -1;
			point = read_n_decode_utf8(input);
			if (point != 0)
				add_char_to_input(cmdline, point);
		}
	}
	return (cmdline->input.reading != -1);
}
