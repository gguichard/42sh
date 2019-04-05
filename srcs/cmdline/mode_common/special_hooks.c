/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_hooks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 01:22:49 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/05 10:04:08 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <term.h>
#include "cmdline.h"

int	handle_end_of_text(t_cmdline *cmdline)
{
	static char	*cr_tcap = NULL;
	static char	*do_tcap = NULL;

	if (cr_tcap == NULL)
		cr_tcap = tgetstr("cr", NULL);
	if (do_tcap == NULL)
		do_tcap = tgetstr("do", NULL);
	cmdline->input.reading = RSTATE_ETX;
	if (cmdline->visual.toggle)
		handle_toggle_visual(cmdline);
	write(STDOUT_FILENO, "^C", 2);
	handle_end_key(cmdline);
	tputs(cr_tcap, 1, t_putchar);
	tputs(do_tcap, 1, t_putchar);
	return (1);
}
