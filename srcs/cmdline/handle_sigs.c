/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_sigs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/24 15:58:30 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/28 15:46:48 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <term.h>
#include "cmdline.h"

void	handle_sigwinch(int sig)
{
	t_cmdline	*cmdline;

	(void)sig;
	ft_printf("Sigwinch handler\n");
	cmdline = g_cmdline;
	go_to_offset(cmdline, 0);
	update_winsize(cmdline);
	tputs(tgetstr("cr", NULL), 1, t_putchar);
	tputs(tgetstr("cd", NULL), cmdline->winsize.ws_row - cmdline->cursor.y
			, t_putchar);
	print_prompt_and_cmdline(cmdline);
}
