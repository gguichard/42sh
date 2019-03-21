/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   select_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 22:06:22 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/19 11:25:06 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <wchar.h>
#include <term.h>
#include "cmdline.h"

static void	print_visual_select(t_cmdline *cmdline, int off_s, int off_e)
{
	go_to_offset(cmdline, off_s);
	if (cmdline->visual.toggle)
		tputs(tgetstr("mr", NULL), 1, t_putchar);
	write(STDOUT_FILENO, cmdline->input.buffer + off_s
			, (off_e - off_s) * sizeof(wint_t));
	if (cmdline->visual.toggle)
		tputs(tgetstr("me", NULL), 1, t_putchar);
}

static void	print_visual_unselect(t_cmdline *cmdline, int old_off, int off)
{
	int	min_off;
	int	max_off;

	min_off = ft_min(old_off, off);
	max_off = ft_max(old_off, off);
	go_to_offset(cmdline, min_off);
	write(STDOUT_FILENO, cmdline->input.buffer + min_off
			, (max_off - min_off) * sizeof(wint_t));
}

void		update_visual_select(t_cmdline *cmdline)
{
	static int	old_off_s = -1;
	static int	old_off_e = -1;
	int			off_s;
	int			off_e;

	if (!cmdline->visual.toggle)
	{
		old_off_s = -1;
		old_off_e = -1;
	}
	else
	{
		off_s = ft_min(cmdline->visual.start_offset, cmdline->input.offset);
		if (old_off_s != -1 && old_off_s != off_s)
			print_visual_unselect(cmdline, old_off_s, off_s);
		off_e = ft_max(cmdline->visual.start_offset, cmdline->input.offset);
		if (old_off_e != -1 && old_off_e != off_e)
			print_visual_unselect(cmdline, old_off_e, off_e);
		if (off_s != off_e)
			print_visual_select(cmdline, off_s, off_e);
		old_off_s = off_s;
		old_off_e = off_e;
		go_to_cursor_pos(cmdline->cursor);
	}
}
