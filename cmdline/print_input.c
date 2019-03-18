/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/17 00:13:25 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/18 23:08:17 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <wchar.h>
#include <term.h>
#include "cmdline.h"

static void	print_mbstr(const wchar_t *buffer, int len)
{
	char	*str;
	int		offset;
	int		wlen;

	str = (char *)malloc(len * sizeof(wint_t));
	if (str == NULL)
		write(STDOUT_FILENO, buffer, len * sizeof(wint_t));
	else
	{
		offset = 0;
		while (len > 0)
		{
			wlen = ft_wcharlen(*buffer);
			ft_memcpy(str + offset, buffer, wlen);
			offset += wlen;
			buffer++;
			len--;
		}
		write(STDOUT_FILENO, str, offset);
		free(str);
	}
}

static void	update_end_cursor_print(t_cmdline *cmdline, t_cursor *end_cursor
		, const wchar_t *buffer, int offset)
{
	static const char	*ce_tcap = NULL;

	if (ce_tcap == NULL)
		ce_tcap = tgetstr("ce", NULL);
	if (offset > 0)
	{
		end_cursor->x += offset;
		end_cursor->y += end_cursor->x / ft_max(1, cmdline->winsize.ws_col);
		end_cursor->x %= ft_max(1, cmdline->winsize.ws_col);
		print_mbstr(buffer, offset);
	}
	tputs(ce_tcap, 1, t_putchar);
}

static void	fix_outbound_cursor(t_cmdline *cmdline, t_cursor *end_cursor)
{
	int	scroll_up;

	if (end_cursor->y >= cmdline->winsize.ws_row)
	{
		scroll_up = (end_cursor->y - cmdline->winsize.ws_row) + 1;
		cmdline->cursor.y -= scroll_up;
		if (end_cursor->x == 0)
		{
			tputs(tgetstr("cr", NULL), 1, t_putchar);
			tputs(tgetstr("sf", NULL), 1, t_putchar);
		}
	}
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
		update_end_cursor_print(cmdline, &end_cursor, buffer, offset);
		if (eol != NULL)
		{
			if (offset != 0 && end_cursor.x == 0)
				print_go_next_line();
			write(STDOUT_FILENO, "\n", 1);
			end_cursor.x = 0;
			end_cursor.y += 1;
			offset++;
		}
		buffer += offset;
		buff_len -= offset;
	}
	fix_outbound_cursor(cmdline, &end_cursor);
}

void		update_cmdline_after_offset(t_cmdline *cmdline)
{
	if (cmdline->input.offset != cmdline->input.size)
	{
		print_line_by_line(cmdline, cmdline->cursor);
		go_to_cursor_pos(cmdline->cursor);
	}
}
