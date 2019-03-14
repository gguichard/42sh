/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/14 10:11:35 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/14 12:39:11 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmdline.h"

int	get_rightmost_column(t_cmdline *cmdline, int offset)
{
	int	col;

	if (cmdline->input.buffer[offset] != '\n')
		return (cmdline->winsize.ws_col - 1);
	else
	{
		col = 0;
		while (offset > 0 && cmdline->input.buffer[--offset] != '\n')
			col++;
		if (offset == 0)
			col += cmdline->prompt.offset;
		if (cmdline->winsize.ws_col <= 0)
			return (0);
		else
			return (col % cmdline->winsize.ws_col);
	}
}
