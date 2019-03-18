/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   misc_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 20:23:18 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/18 23:22:11 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <term.h>
#include "cmdline.h"

int	handle_clear(t_cmdline *cmdline)
{
	tputs(tgetstr("cl", NULL), 1, t_putchar);
	ft_printf("TODO");
	cmdline->input.offset = 0;
	cmdline->cursor.x = 0;
	cmdline->cursor.y = 0;
	cmdline->row = 0;
	return (1);
}

int	handle_bell(t_cmdline *cmdline)
{
	(void)cmdline;
	tputs(tgetstr("bl", NULL), 1, t_putchar);
	return (1);
}
