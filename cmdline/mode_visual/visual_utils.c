/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visual_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 14:50:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/15 12:12:22 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <term.h>
#include "cmdline.h"

void	update_visual_select(t_cmdline *cmdline)
{
	int	off_s;
	int	off_e;

	go_to_offset(cmdline, 0);
	write(STDOUT_FILENO, cmdline->input.buffer, cmdline->input.size);
	if (cmdline->visual.toggle)
	{
		off_s = ft_min(cmdline->visual.start_offset, cmdline->input.offset);
		off_e = ft_max(cmdline->visual.start_offset, cmdline->input.offset);
		go_to_offset(cmdline, off_s);
		tputs(tgetstr("mr", NULL), 1, t_putchar);
		write(STDOUT_FILENO, cmdline->input.buffer + off_s, off_e - off_s);
		tputs(tgetstr("me", NULL), 1, t_putchar);
	}
	go_to_cursor_pos(cmdline->cursor);
}

int		vm_copy(t_cmdline *cmdline, int cut_hook)
{
	int			off_s;
	int			off_e;
	int			off;
	t_cursor	new_cursor;

	ft_strdel(&cmdline->visual.clipboard);
	off_s = ft_min(cmdline->input.offset, cmdline->visual.start_offset);
	off_e = ft_max(cmdline->input.offset, cmdline->visual.start_offset);
	off = off_e - off_s;
	if (off == 0)
		return (0);
	cmdline->visual.clipboard = ft_strsub(cmdline->input.buffer, off_s, off);
	if (cmdline->visual.clipboard != NULL && cut_hook)
	{
		ft_memcpy(cmdline->input.buffer + off_s
				, cmdline->input.buffer + off_e
				, cmdline->input.size - off_e);
		cmdline->input.offset = off_s;
		cmdline->input.size -= off;
		new_cursor = go_to_offset(cmdline, off_s);
		cmdline->row -= (cmdline->cursor.y - new_cursor.y);
		cmdline->cursor = new_cursor;
		clear_after_cursor(new_cursor, cmdline->winsize);
	}
	handle_toggle_visual(cmdline);
	return (1);
}

int		vm_paste(t_cmdline *cmdline, int paste_after_cursor)
{
	const char	*clipboard;

	clipboard = cmdline->visual.clipboard;
	if (clipboard == NULL)
		return (0);
	if (paste_after_cursor)
		handle_move_right(cmdline);
	while (*clipboard != '\0')
	{
		add_char_to_input(&cmdline->input, *clipboard);
		write_char_in_cmdline(cmdline, *clipboard);
		clipboard++;
	}
	handle_toggle_visual(cmdline);
	return (1);
}
