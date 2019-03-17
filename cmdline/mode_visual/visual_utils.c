/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visual_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 14:50:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/17 15:51:00 by gguichar         ###   ########.fr       */
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

void		update_visual_select(t_cmdline *cmdline)
{
	static int	old_off_s = -1;
	static int	old_off_e = -1;
	int			off_s;
	int			off_e;

	off_s = ft_min(cmdline->visual.start_offset, cmdline->input.offset);
	off_e = ft_max(cmdline->visual.start_offset, cmdline->input.offset);
	if (!cmdline->visual.toggle)
	{
		old_off_s = -1;
		old_off_e = -1;
	}
	else
	{
		if (old_off_s != -1 && old_off_e != -1)
		{
			go_to_offset(cmdline, old_off_s);
			write(STDOUT_FILENO, cmdline->input.buffer + old_off_s
					, (old_off_e - old_off_s) * sizeof(wint_t));
		}
		old_off_s = off_s;
		old_off_e = off_e;
	}
	print_visual_select(cmdline, off_s, off_e);
	go_to_cursor_pos(cmdline->cursor);
}

static void	vm_cut_hook(t_cmdline *cmdline, int off_s, int off_e, int off)
{
	t_cursor	new_cursor;

	ft_memcpy(cmdline->input.buffer + off_s, cmdline->input.buffer + off_e
			, cmdline->input.size - off_e + 1);
	cmdline->input.offset = off_s;
	cmdline->input.size -= off;
	new_cursor = go_to_offset(cmdline, off_s);
	cmdline->row -= (cmdline->cursor.y - new_cursor.y);
	cmdline->cursor = new_cursor;
	cmdline->visual.start_offset = off_s;
	clear_after_cursor(new_cursor, cmdline->winsize);
	write(STDOUT_FILENO, cmdline->input.buffer + off_s
			, (cmdline->input.size - off_s) * sizeof(wint_t));
}

int			vm_copy(t_cmdline *cmdline, int cut_hook)
{
	int			off_s;
	int			off_e;
	int			off;

	ft_memdel((void **)&cmdline->visual.clipboard);
	off_s = ft_min(cmdline->input.offset, cmdline->visual.start_offset);
	off_e = ft_max(cmdline->input.offset, cmdline->visual.start_offset);
	off = off_e - off_s;
	if (off > 0)
	{
		cmdline->visual.clipboard = (wchar_t *)malloc((off + 1)
				* sizeof(wint_t));
		if (cmdline->visual.clipboard != NULL)
		{
			ft_memcpy(cmdline->visual.clipboard, cmdline->input.buffer + off_s
					, off * sizeof(wint_t));
			cmdline->visual.clipboard[off] = L'\0';
			if (cut_hook)
				vm_cut_hook(cmdline, off_s, off_e, off);
		}
	}
	handle_toggle_visual(cmdline);
	return (1);
}

int			vm_paste(t_cmdline *cmdline, int paste_after_cursor)
{
	const wchar_t	*clipboard;

	clipboard = cmdline->visual.clipboard;
	if (clipboard == NULL || clipboard[0] == L'\0')
		return (0);
	if (paste_after_cursor)
		handle_move_right(cmdline);
	while (*clipboard != L'\0')
	{
		add_char_to_input(cmdline, *clipboard);
		clipboard++;
	}
	handle_toggle_visual(cmdline);
	return (1);
}
