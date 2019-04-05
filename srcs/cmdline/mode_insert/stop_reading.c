/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop_reading.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 10:15:14 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/05 10:02:20 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <term.h>
#include "cmdline.h"

int	handle_cmdline_end(t_cmdline *cmdline)
{
	static char	*cr_tcap = NULL;
	static char	*do_tcap = NULL;

	if (cr_tcap == NULL)
		cr_tcap = tgetstr("cr", NULL);
	if (do_tcap == NULL)
		do_tcap = tgetstr("do", NULL);
	cmdline->input.reading = RSTATE_END;
	handle_end_key(cmdline);
	tputs(cr_tcap, 1, t_putchar);
	tputs(do_tcap, 1, t_putchar);
	return (1);
}

int	handle_eot_key(t_cmdline *cmdline)
{
	if (cmdline->input.size != 0)
		return (0);
	cmdline->input.reading = RSTATE_EOT;
	return (1);
}
