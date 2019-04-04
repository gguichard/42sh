/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_sigs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/24 15:58:30 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/02 19:02:25 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <term.h>
#include "cmdline.h"

void	handle_sigwinch(int sig)
{
	static char	*cr_tcap = NULL;
	static char	*cd_tcap = NULL;
	t_cmdline	*cmdline;

	if (cr_tcap == NULL)
		cr_tcap = tgetstr("cr", NULL);
	if (cd_tcap == NULL)
		cd_tcap = tgetstr("cd", NULL);
	(void)sig;
	cmdline = g_cmdline;
	go_to_offset(cmdline, 0);
	update_winsize(cmdline);
	tputs(cr_tcap, 1, t_putchar);
	tputs(cd_tcap, cmdline->winsize.ws_row - cmdline->cursor.y, t_putchar);
	print_prompt_and_cmdline(cmdline);
}
