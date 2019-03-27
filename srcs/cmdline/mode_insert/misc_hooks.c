/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 20:23:18 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/27 01:03:13 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <term.h>
#include "cmdline.h"

int	handle_clear(t_cmdline *cmdline)
{
	tputs(tgetstr("cl", NULL), 1, t_putchar);
	print_prompt_and_cmdline(cmdline);
	return (1);
}

int	handle_bell(t_cmdline *cmdline)
{
	(void)cmdline;
	tputs(tgetstr("bl", NULL), 1, t_putchar);
	return (1);
}
