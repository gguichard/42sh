/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 22:06:22 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/06 16:12:14 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <term.h>
#include "cmdline.h"

static void	print_visual_select(t_cmdline *cmdline, int off_s, int off_e)
{
	static char	*mr_tcap = NULL;
	static char	*me_tcap = NULL;

	if (mr_tcap == NULL)
		mr_tcap = tgetstr("mr", NULL);
	if (me_tcap == NULL)
		me_tcap = tgetstr("me", NULL);
	go_to_offset(cmdline, off_s);
	if (cmdline->visual.toggle)
		tputs(mr_tcap, 1, t_putchar);
	print_line_by_line(cmdline, cmdline->input.buffer + off_s
			, off_e - off_s, 0);
	if (cmdline->visual.toggle)
		tputs(me_tcap, 1, t_putchar);
}

static void	print_visual_unselect(t_cmdline *cmdline, int old_off, int off)
{
	int	min_off;
	int	max_off;

	min_off = ft_min(old_off, off);
	max_off = ft_max(old_off, off);
	go_to_offset(cmdline, min_off);
	print_line_by_line(cmdline, cmdline->input.buffer + min_off
			, max_off - min_off, 0);
}

void		update_visual_select(t_cmdline *cmdline)
{
	static int	old_off_s = -1;
	static int	old_off_e = -1;
	int			off_s;
	int			off_e;
	t_cursor	saved_cursor;

	if (!cmdline->visual.toggle)
	{
		old_off_s = -1;
		old_off_e = -1;
	}
	else
	{
		saved_cursor = cmdline->cursor;
		off_s = ft_min(cmdline->visual.start_offset, cmdline->input.offset);
		if (old_off_s != -1 && old_off_s != off_s)
			print_visual_unselect(cmdline, old_off_s, off_s);
		off_e = ft_max(cmdline->visual.start_offset, cmdline->input.offset);
		if (old_off_e != -1 && old_off_e != off_e)
			print_visual_unselect(cmdline, old_off_e, off_e);
		(off_s != off_e) ? print_visual_select(cmdline, off_s, off_e) : 0;
		old_off_s = off_s;
		old_off_e = off_e;
		go_to_cursor_pos(cmdline, saved_cursor);
	}
}
