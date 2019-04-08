/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   konami_code.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 11:05:29 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/08 19:01:30 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term.h>
#include "cmdline.h"

int	handle_konami_code(t_cmdline *cmdline)
{
	static char	*cr_tcap = NULL;
	t_cursor	saved_cursor;

	if (cr_tcap == NULL)
		cr_tcap = tgetstr("cr", NULL);
	cmdline->konami_code = !cmdline->konami_code;
	saved_cursor = cmdline->cursor;
	go_to_offset(cmdline, 0);
	tputs(cr_tcap, 1, t_putchar);
	print_prompt_and_cmdline(cmdline);
	go_to_cursor_pos(cmdline, saved_cursor);
	return (1);
}
