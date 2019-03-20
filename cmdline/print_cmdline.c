/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cmdline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/17 00:13:25 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/20 01:43:53 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <wchar.h>
#include <term.h>
#include "cmdline.h"

static void	print_one_line(t_cmdline *cmdline, t_cursor *end_cursor
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

static void	scroll_if_needed(t_cmdline *cmdline, t_cursor *sc, t_cursor *ec)
{
	int	scroll_up;

	if (ec->y >= cmdline->winsize.ws_row)
	{
		scroll_up = (ec->y - cmdline->winsize.ws_row) + 1;
		cmdline->cursor.y -= scroll_up;
		if (sc != NULL)
			sc->y -= scroll_up;
		if (ec->x == 0)
		{
			tputs(tgetstr("cr", NULL), 1, t_putchar);
			tputs(tgetstr("sf", NULL), cmdline->winsize.ws_row, t_putchar);
			sc->y -= 1;
		}
	}
}

static void	print_line_by_line(t_cmdline *cmdline, int off_start
		, t_cursor *sc, t_cursor *ec)
{
	const wchar_t	*buffer;
	const wchar_t	*eol;
	int				buff_len;
	int				offset;

	buffer = cmdline->input.buffer + off_start;
	buff_len = cmdline->input.size - off_start;
	while (buff_len > 0)
	{
		eol = ft_wstrchr(buffer, L'\n');
		offset = (eol == NULL) ? buff_len : (eol - buffer);
		print_one_line(cmdline, ec, buffer, offset);
		if (eol != NULL)
		{
			if (offset != 0 && ec->x == 0)
				print_next_line_tcaps();
			write(STDOUT_FILENO, "\n", 1);
			ec->x = 0;
			ec->y += 1;
			offset++;
		}
		buffer += offset;
		buff_len -= offset;
	}
	scroll_if_needed(cmdline, sc, ec);
}

/*
 ** Met a jour l'affichage de la ligne de commande apres l'offset indique
 ** dans l'input de celle-ci.
 */
void		update_cmdline_at_offset(t_cmdline *cmdline)
{
	t_cursor	end_cursor;

	if (cmdline->input.offset != cmdline->input.size)
	{
		end_cursor = cmdline->cursor;
		print_line_by_line(cmdline, cmdline->input.offset, NULL, &end_cursor);
		go_to_cursor_pos(cmdline->cursor);
	}
	print_big_cmdline_prompt(cmdline);
}

/*
 ** Affiche la totalite de la ligne de commande.
 ** ATTENTION : le curseur dans la ligne de commande doit etre defini
 ** a la position ou celle-ci va commencee a etre ecrite.
 */
void		print_cmdline(t_cmdline *cmdline)
{
	t_cursor	start_cursor;
	t_cursor	end_cursor;
	int			offset;

	start_cursor = cmdline->cursor;
	end_cursor = cmdline->cursor;
	print_line_by_line(cmdline, 0, &start_cursor, &end_cursor);
	offset = 0;
	cmdline->row = 0;
	while (offset < cmdline->input.offset)
	{
		if (cmdline->input.buffer[offset] != '\n'
				&& (start_cursor.x + 1) < cmdline->winsize.ws_col)
			start_cursor.x += 1;
		else
		{
			start_cursor.x = 0;
			start_cursor.y += 1;
			cmdline->row += 1;
		}
		offset++;
	}
	cmdline->cursor = start_cursor;
	if (!print_big_cmdline_prompt(cmdline))
		go_to_cursor_pos(start_cursor);
}
