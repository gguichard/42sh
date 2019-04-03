/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 16:26:49 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/03 10:14:54 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <term.h>
#include "cmdline.h"

static void	del_char_on_cursor(t_cmdline *cmdline)
{
	static char	*ce_tcap = NULL;
	char		caller;

	if (ce_tcap == NULL)
		ce_tcap = tgetstr("ce", NULL);
	caller = cmdline->input.buffer[cmdline->input.offset];
	ft_memcpy(cmdline->input.buffer + cmdline->input.offset
			, cmdline->input.buffer + cmdline->input.offset + 1
			, (cmdline->input.size - cmdline->input.offset));
	cmdline->input.size -= 1;
	tputs(ce_tcap, 1, t_putchar);
	update_cmdline_at_offset(cmdline, caller, 1);
}

int			handle_backspace_key(t_cmdline *cmdline)
{
	if (cmdline->input.offset == 0 || !handle_move_left(cmdline))
		return (0);
	del_char_on_cursor(cmdline);
	return (1);
}

int			handle_delete_key(t_cmdline *cmdline)
{
	if (cmdline->input.offset == cmdline->input.size)
		return (0);
	del_char_on_cursor(cmdline);
	return (1);
}
