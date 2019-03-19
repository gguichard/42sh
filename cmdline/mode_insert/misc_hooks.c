/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 20:23:18 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/19 11:01:19 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <term.h>
#include "cmdline.h"

int	handle_clear(t_cmdline *cmdline)
{
	tputs(tgetstr("cl", NULL), 1, t_putchar);
	write(STDOUT_FILENO, cmdline->prompt.str, ft_strlen(cmdline->prompt.str));
	cmdline->row = 0;
	set_cursor_pos(&cmdline->cursor);
	cmdline->prompt.offset = cmdline->cursor.x;
	recompute_cursor(cmdline);
	print_mbstr(cmdline->input.buffer, cmdline->input.size);
	go_to_cursor_pos(cmdline->cursor);
	return (1);
}

int	handle_bell(t_cmdline *cmdline)
{
	(void)cmdline;
	tputs(tgetstr("bl", NULL), 1, t_putchar);
	return (1);
}
