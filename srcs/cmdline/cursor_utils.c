/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 16:00:18 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/30 19:44:42 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <term.h>
#include "cmdline.h"

void	go_to_cursor_pos(t_cmdline *cmdline, t_cursor cursor)
{
	static char	*cr_tcap = NULL;
	static char	*do_tcap = NULL;
	static char	*up_tcap = NULL;
	static char	*ri_tcap = NULL;

	if (cr_tcap == NULL)
		cr_tcap = tgetstr("cr", NULL);
	if (do_tcap == NULL)
		do_tcap = tgetstr("DO", NULL);
	if (up_tcap == NULL)
		up_tcap = tgetstr("UP", NULL);
	if (ri_tcap == NULL)
		ri_tcap = tgetstr("RI", NULL);
	if (cmdline->cursor.x == cursor.x && cmdline->cursor.y == cursor.y)
		return ;
	if (cmdline->cursor.x != 0)
		tputs(cr_tcap, 1, t_putchar);
	if (cmdline->cursor.y < cursor.y)
		tputs(tparm(do_tcap, cursor.y - cmdline->cursor.y), 1, t_putchar);
	if (cmdline->cursor.y > cursor.y)
		tputs(tparm(up_tcap, cmdline->cursor.y - cursor.y), 1, t_putchar);
	cmdline->cursor = cursor;
	if (cursor.x != 0)
		tputs(tparm(ri_tcap, cursor.x), 1, t_putchar);
}
