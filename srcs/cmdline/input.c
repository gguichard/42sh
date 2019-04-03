/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 16:28:07 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/03 10:15:10 by gguichar         ###   ########.fr       */
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
		cmdline->cursor.y += 1;
	}
	update_cmdline_at_offset(cmdline, c, 0);
}

void			add_char_to_input(t_cmdline *cmdline, char c)
{
	if (cmdline->input.size == cmdline->input.capacity)
		return ;
	ft_memmove(cmdline->input.buffer + cmdline->input.offset + 1
			, cmdline->input.buffer + cmdline->input.offset
			, (cmdline->input.size - cmdline->input.offset + 1));
	cmdline->input.buffer[cmdline->input.offset] = c;
	cmdline->input.offset += 1;
	cmdline->input.size += 1;
	write_char_in_cmdline(cmdline, c);
}

static void		print_unterminated_line(t_cmdline *cmdline)
{
	size_t	buff_len;
	char	*buffer;

	buff_len = cmdline->winsize.ws_col - 1;
	buffer = (char *)malloc(buff_len * sizeof(char));
	if (buffer == NULL)
		return ;
	ft_memset(buffer, ' ', buff_len);
	tputs(tgetstr("mr", NULL), 1, t_putchar);
	write(STDOUT_FILENO, "%", 1);
	tputs(tgetstr("me", NULL), 1, t_putchar);
	write(STDOUT_FILENO, buffer, buff_len);
	tputs(tgetstr("cr", NULL), 1, t_putchar);
	tputs(tgetstr("ce", NULL), 1, t_putchar);
	free(buffer);
}

t_rstate		read_input(t_cmdline *cmdline, const char *prompt)
{
	char		input;
	const t_seq	*seq;

	print_unterminated_line(cmdline);
	reset_cmdline(cmdline, prompt);
	while (cmdline->input.reading == RSTATE_READING)
	{
		if (read(STDIN_FILENO, &input, 1) <= 0)
		{
			cmdline->input.reading = RSTATE_EOT;
			break ;
		}
		seq = get_sequence(cmdline, input);
		if (seq != NULL)
			handle_sequence(cmdline, seq);
		else if (cmdline->visual.toggle)
			tputs(tgetstr("bl", NULL), 1, t_putchar);
		else
		{
			cmdline->saved_col = -1;
			if (ft_isprint(input))
				add_char_to_input(cmdline, input);
		}
		if (input != '\t')
			cmdline->ac_flag = 0;
	}
	return (cmdline->input.reading);
}
