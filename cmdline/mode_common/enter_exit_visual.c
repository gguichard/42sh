/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enter_exit_visual.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 10:57:56 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/19 11:25:17 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <term.h>
#include "cmdline.h"

int	handle_toggle_visual(t_cmdline *cmdline)
{
	cmdline->visual.toggle = !cmdline->visual.toggle;
	tputs(tgetstr(cmdline->visual.toggle ? "vi" : "ve", NULL), 1, t_putchar);
	if (!cmdline->visual.toggle)
		update_visual_select(cmdline);
	go_to_cursor_pos((t_cursor){0, cmdline->cursor.y - cmdline->row});
	tputs(tgetstr("cd", NULL), 1, t_putchar);
	if (cmdline->visual.toggle)
	{
		cmdline->visual.start_offset = cmdline->input.offset;
		write(STDOUT_FILENO, "(visual) ", 9);
	}
	write(STDOUT_FILENO, cmdline->prompt.str, ft_strlen(cmdline->prompt.str));
	set_cursor_pos(&cmdline->cursor);
	cmdline->prompt.offset = cmdline->cursor.x;
	cmdline->row = 0;
	recompute_cursor(cmdline);
	if (cmdline->input.size > 0)
	{
		print_mbstr(cmdline->input.buffer, cmdline->input.size);
		go_to_cursor_pos(cmdline->cursor);
	}
	return (1);
}