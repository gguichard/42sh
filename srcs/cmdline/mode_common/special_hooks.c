/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_hooks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 01:22:49 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/04 00:31:28 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "cmdline.h"

int	handle_end_of_text(t_cmdline *cmdline)
{
	if (cmdline->visual.toggle)
		handle_toggle_visual(cmdline);
	write(STDOUT_FILENO, "^C", 2);
	handle_end_key(cmdline);
	write(STDOUT_FILENO, "\n", 1);
	cmdline->input.reading = RSTATE_ETX;
	return (1);
}
