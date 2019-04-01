/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 20:23:18 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/30 20:14:38 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term.h>
#include "cmdline.h"

int	handle_clear(t_cmdline *cmdline)
{
	static char	*cl_tcap = NULL;

	if (cl_tcap == NULL)
		cl_tcap = tgetstr("cl", NULL);
	tputs(cl_tcap, 1, t_putchar);
	print_prompt_and_cmdline(cmdline);
	return (1);
}

int	handle_bell(t_cmdline *cmdline)
{
	static char	*bl_tcap = NULL;

	if (bl_tcap == NULL)
		bl_tcap = tgetstr("bl", NULL);
	(void)cmdline;
	tputs(bl_tcap, 1, t_putchar);
	return (1);
}
