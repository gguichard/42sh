/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cmdline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/17 00:13:25 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/03 10:36:03 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <term.h>
#include "cmdline.h"

void	update_cmdline_at_offset(t_cmdline *cmdline, char caller
		, int is_deletion)
{
	static char	*cr_tcap = NULL;
	static char	*do_tcap = NULL;
	static char	*ce_tcap = NULL;
	t_cursor	saved_cursor;

	if (cr_tcap == NULL)
		cr_tcap = tgetstr("cr", NULL);
	if (do_tcap == NULL)
		do_tcap = tgetstr("do", NULL);
	if (ce_tcap == NULL)
		ce_tcap = tgetstr("ce", NULL);
	if (cmdline->input.offset != cmdline->input.size)
	{
		saved_cursor = cmdline->cursor;
		print_line_by_line(cmdline, cmdline->input.offset);
		if (is_deletion && (caller == '\n' || cmdline->cursor.x == 0))
		{
			if (caller == '\n')
			{
				tputs(cr_tcap, 1, t_putchar);
				tputs(do_tcap, 1, t_putchar);
				cmdline->cursor.x = 0;
				cmdline->cursor.y += 1;
			}
			tputs(ce_tcap, 1, t_putchar);
		}
		go_to_cursor_pos(cmdline, saved_cursor);
	}
}

void	print_prompt_and_cmdline(t_cmdline *cmdline)
{
	size_t	total;

	total = 0;
	if (cmdline->visual.toggle)
		total += ft_max(write(STDOUT_FILENO, "(visual) ", 9), 0);
	total += ft_max(write(STDOUT_FILENO, cmdline->prompt.str
				, ft_strlen(cmdline->prompt.str)), 0);
	cmdline->cursor.x = total % ft_max(cmdline->winsize.ws_col, 1);
	cmdline->cursor.y = 0;
	cmdline->prompt.offset = cmdline->cursor.x;
	print_only_cmdline(cmdline);
}
