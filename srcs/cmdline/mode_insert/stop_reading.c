/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop_reading.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 10:15:14 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/27 01:40:27 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "cmdline.h"

int	handle_cmdline_end(t_cmdline *cmdline)
{
	handle_end_key(cmdline);
	write(STDOUT_FILENO, "\n", 1);
	cmdline->input.reading = RSTATE_END;
	return (1);
}

int	handle_eot_key(t_cmdline *cmdline)
{
	if (cmdline->input.size != 0)
		return (0);
	cmdline->input.reading = RSTATE_EOT;
	return (1);
}
