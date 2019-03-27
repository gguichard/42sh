/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enter_exit_visual.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 10:57:56 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/27 01:08:55 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <term.h>
#include "cmdline.h"

int	handle_toggle_visual(t_cmdline *cmdline)
{
	cmdline->visual.toggle = !cmdline->visual.toggle;
	tputs(tgetstr(cmdline->visual.toggle ? "vi" : "ve", NULL), 1, t_putchar);
	if (cmdline->visual.toggle)
		cmdline->visual.start_offset = cmdline->input.offset;
	else
		update_visual_select(cmdline);
	go_to_cursor_pos((t_cursor){
			0, ft_max(0, cmdline->cursor.y - cmdline->row)});
	tputs(tgetstr("cd", NULL), 1, t_putchar);
	print_prompt_and_cmdline(cmdline);
	return (1);
}
