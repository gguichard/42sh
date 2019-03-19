/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop_reading.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/19 10:15:14 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/19 10:16:04 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmdline.h"

int	handle_eot_key(t_cmdline *cmdline)
{
	if (cmdline->input.size != 0)
		return (0);
	cmdline->input.reading = 0;
	return (1);
}
