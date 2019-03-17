/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/17 00:13:25 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/17 16:49:39 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <wchar.h>
#include <term.h>
#include "cmdline.h"

static void	update_cursor_next_line(t_cursor *end_cursor)
{
	write(STDOUT_FILENO, "\n", 1);
	end_cursor->x = 0;
	end_cursor->y += 1;
}

static void	update_end_cursor_print(t_cmdline *cmdline, t_cursor *end_cursor
		, int offset, const wchar_t *buffer)
{
	static const char	*ce_tcap = NULL;

	if (ce_tcap == NULL)
		ce_tcap = tgetstr("ce", NULL);
	end_cursor->x += offset;
	end_cursor->y += end_cursor->x / ft_max(1, cmdline->winsize.ws_col);
	end_cursor->x %= ft_max(1, cmdline->winsize.ws_col);
	write(STDOUT_FILENO, buffer, offset * sizeof(wint_t));
	tputs(ce_tcap, 1, t_putchar);
}

static void	print_line_by_line(t_cmdline *cmdline, t_cursor end_cursor)
{
	const wchar_t	*buffer;
	const wchar_t	*eol;
	int				buff_len;
	int				offset;

	buffer = cmdline->input.buffer + cmdline->input.offset;
	buff_len = cmdline->input.size - cmdline->input.offset;
	while (buff_len > 0)
	{
		eol = ft_wstrchr(buffer, L'\n');
		offset = (eol == NULL) ? buff_len : (eol - buffer);
		update_end_cursor_print(cmdline, &end_cursor, offset, buffer);
		if (eol != NULL)
		{
			update_cursor_next_line(&end_cursor);
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
