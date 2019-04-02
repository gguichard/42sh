/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cmdline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 19:50:30 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/30 20:55:46 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <term.h>
#include "cmdline.h"

static void	print_one_line(t_cmdline *cmdline, const char *buffer, int offset)
{
	static const char	*ce_tcap = NULL;

	if (ce_tcap == NULL)
		ce_tcap = tgetstr("ce", NULL);
	if (offset > 0)
		print_cmdline_str(cmdline, buffer, offset);
	tputs(ce_tcap, 1, t_putchar);
}

static void	print_next_line_tcaps(void)
{
	static char	*cr_tcap = NULL;
	static char	*do_tcap = NULL;
	static char	*ce_tcap = NULL;

	if (cr_tcap == NULL)
		cr_tcap = tgetstr("cr", NULL);
	if (do_tcap == NULL)
		do_tcap = tgetstr("do", NULL);
	if (ce_tcap == NULL)
		ce_tcap = tgetstr("ce", NULL);
	tputs(cr_tcap, 1, t_putchar);
	tputs(do_tcap, 1, t_putchar);
	tputs(ce_tcap, 1, t_putchar);
}

void		print_line_by_line(t_cmdline *cmdline, int off_start)
{
	const char	*buffer;
	const char	*eol;
	int			buff_len;
	int			offset;

	buffer = cmdline->input.buffer + off_start;
	buff_len = cmdline->input.size - off_start;
	while (buff_len > 0)
	{
		eol = ft_strchr(buffer, '\n');
		offset = (eol == NULL) ? buff_len : (eol - buffer);
		print_one_line(cmdline, buffer, offset);
		if (eol != NULL)
		{
			if (offset != 0 && cmdline->cursor.x == 0)
				print_next_line_tcaps();
			write(STDOUT_FILENO, "\n", 1);
			cmdline->cursor.x = 0;
			cmdline->cursor.y += 1;
			offset++;
		}
		buffer += offset;
		buff_len -= offset;
	}
}

void		print_cmdline_str(t_cmdline *cmdline, const char *buffer
, size_t len)
{
	static int	color = 1;
	size_t		offset;

	cmdline->cursor.x += len;
	cmdline->cursor.y += cmdline->cursor.x / ft_max(1, cmdline->winsize.ws_col);
	cmdline->cursor.x %= ft_max(1, cmdline->winsize.ws_col);
	if (cmdline->cursor.x == 0)
	{
		cmdline->cursor.x = cmdline->winsize.ws_col - 1;
		cmdline->cursor.y -= 1;
	}
	if (!cmdline->konami_code)
		write(STDOUT_FILENO, buffer, len);
	else
	{
		offset = 0;
		while (offset < len)
		{
			ft_printf("\033[38;5;%dm%c\033[0m", color, buffer[offset]);
			color = (color % 232) + 1;
			offset++;
		}
	}
}

void		print_only_cmdline(t_cmdline *cmdline)
{
	t_cursor	start_cursor;
	int			offset;

	start_cursor = cmdline->cursor;
	print_line_by_line(cmdline, 0);
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
