/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cmdline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/17 00:13:25 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/30 19:59:54 by gguichar         ###   ########.fr       */
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

static void	print_line_by_line(t_cmdline *cmdline, int off_start)
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

/*
 ** Met a jour l'affichage de la ligne de commande apres l'offset indique
 ** dans l'input de celle-ci.
 */

void		update_cmdline_at_offset(t_cmdline *cmdline)
{
	t_cursor	saved_cursor;

	if (cmdline->input.offset != cmdline->input.size)
	{
		saved_cursor = cmdline->cursor;
		print_line_by_line(cmdline, cmdline->input.offset);
		go_to_cursor_pos(cmdline, saved_cursor);
	}
}

/*
 ** Affiche la totalite de la ligne de commande.
 ** ATTENTION : le curseur dans la ligne de commande doit etre defini
 ** a la position ou celle-ci va commencee a etre ecrite.
 */

void		print_cmdline(t_cmdline *cmdline)
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
