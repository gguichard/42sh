/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 16:28:07 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/17 17:29:52 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>
#include <term.h>
#include "cmdline.h"

static void		write_char_in_cmdline(t_cmdline *cmdline, wint_t c)
{
	if (c == '\n')
		tputs(tgetstr("ce", NULL), 1, t_putchar);
	write(STDOUT_FILENO, &c, sizeof(wint_t));
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

void			add_char_to_input(t_cmdline *cmdline, wint_t c)
{
	int		new_cap;
	wchar_t	*new_buffer;

	if (cmdline->input.size == cmdline->input.capacity)
	{
		new_cap = cmdline->input.capacity + INPUT_SIZE_INCR;
		new_buffer = (wchar_t *)malloc((new_cap + 1) * sizeof(wint_t));
		if (new_buffer == NULL)
			return ;
		if (cmdline->input.buffer != NULL)
			ft_memcpy(new_buffer, cmdline->input.buffer
					, cmdline->input.size * sizeof(wint_t));
		new_buffer[cmdline->input.size] = L'\0';
		cmdline->input.capacity = new_cap;
		cmdline->input.buffer = new_buffer;
	}
	if (cmdline->input.offset != cmdline->input.size)
		ft_memmove(cmdline->input.buffer + cmdline->input.offset + 1
				, cmdline->input.buffer + cmdline->input.offset
				, (cmdline->input.size - cmdline->input.offset + 1)
				* sizeof(wint_t));
	cmdline->input.buffer[cmdline->input.offset] = c;
	cmdline->input.offset += 1;
	cmdline->input.size += 1;
	write_char_in_cmdline(cmdline, c);
}

static wint_t	get_full_unichar(unsigned char input)
{
	wint_t	unichar;
	int		additional_len;

	unichar = (wint_t)input;
	if (input & (1 << 7))
	{
		additional_len = 1;
		if (input & (1 << 5))
		{
			additional_len++;
			if (input & (1 << 4))
				additional_len++;
		}
		if (read(STDIN_FILENO, ((unsigned char *)&unichar) + 1
					, additional_len) != additional_len)
			return (0);
	}
	return (unichar);
}

void			read_input(t_cmdline *cmdline)
{
	unsigned char	input;
	const char		*seq;
	wint_t			unichar;

	while (read(STDIN_FILENO, &input, sizeof(char)) && input != 4)
	{
		if ((seq = get_sequence(cmdline, input)) != NULL)
			handle_sequence(cmdline, seq);
		else if (!cmdline->visual.toggle)
		{
			cmdline->saved_col = -1;
			unichar = get_full_unichar(input);
			if (unichar != 0)
				add_char_to_input(cmdline, unichar);
		}
	}
}
