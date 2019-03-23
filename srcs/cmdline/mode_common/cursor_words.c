/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_words.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 19:56:15 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/13 19:52:05 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmdline.h"

int	handle_prev_word(t_cmdline *cmdline)
{
	cmdline->saved_col = -1;
	if (cmdline->input.offset == 0)
		return (0);
	while (cmdline->input.offset > 0
			&& (cmdline->input.buffer[cmdline->input.offset - 1] == ' '
				|| cmdline->input.buffer[cmdline->input.offset - 1] == '\n')
			&& handle_move_left(cmdline))
		continue ;
	while (cmdline->input.offset > 0
			&& cmdline->input.buffer[cmdline->input.offset - 1] != ' '
			&& cmdline->input.buffer[cmdline->input.offset - 1] != '\n'
			&& handle_move_left(cmdline))
		continue ;
	return (1);
}

int	handle_next_word(t_cmdline *cmdline)
{
	cmdline->saved_col = -1;
	if (cmdline->input.offset == cmdline->input.size)
		return (0);
	while (cmdline->input.offset < cmdline->input.size
			&& cmdline->input.buffer[cmdline->input.offset] != ' '
			&& cmdline->input.buffer[cmdline->input.offset] != '\n'
			&& handle_move_right(cmdline))
		continue ;
	while (cmdline->input.offset < cmdline->input.size
			&& (cmdline->input.buffer[cmdline->input.offset] == ' '
				|| cmdline->input.buffer[cmdline->input.offset] == '\n')
			&& handle_move_right(cmdline))
		continue ;
	return (1);
}
