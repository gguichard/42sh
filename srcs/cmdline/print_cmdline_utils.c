/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cmdline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 19:50:30 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/06 16:13:30 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <term.h>
#include "cmdline.h"

static void	print_cmdline_buffer(t_cmdline *cmdline, const char *buffer
		, size_t len)
{
	static int	color = 1;
	size_t		offset;

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

static void	print_one_line(t_cmdline *cmdline, const char *buffer
		, size_t offset, int refresh_needed)
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
	if (offset > 0)
	{
		print_cmdline_buffer(cmdline, buffer, offset);
		cmdline->cursor.x += offset;
		cmdline->cursor.y += cmdline->cursor.x / ft_max(cmdline->winsize.ws_col
				, 1);
		cmdline->cursor.x %= ft_max(cmdline->winsize.ws_col, 1);
		if (cmdline->cursor.x == 0)
		{
			tputs(cr_tcap, 1, t_putchar);
			tputs(do_tcap, 1, t_putchar);
		}
	}
	if (refresh_needed)
		tputs(ce_tcap, 1, t_putchar);
}

void		print_line_by_line(t_cmdline *cmdline, const char *buffer
		, size_t buff_len, int refresh_needed)
{
	const char	*eol;
	size_t		offset;

	while (buff_len > 0)
	{
		eol = ft_memchr(buffer, '\n', buff_len);
		offset = (eol == NULL ? buff_len : (size_t)(eol - buffer));
		print_one_line(cmdline, buffer, offset, refresh_needed);
		if (eol != NULL)
		{
			write(STDOUT_FILENO, "\n", 1);
			cmdline->cursor.x = 0;
			cmdline->cursor.y += 1;
			offset++;
		}
		buffer += offset;
		buff_len -= offset;
	}
}
