/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_words.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/12 19:56:15 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/24 15:38:54 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "cmdline.h"

int	handle_prev_word(t_cmdline *cmdline)
{
	cmdline->saved_col = -1;
	if (cmdline->input.offset == 0)
		return (0);
	while (cmdline->input.offset > 0
			&& ft_isspace(cmdline->input.buffer[cmdline->input.offset - 1])
			&& handle_move_left(cmdline))
		continue ;
	while (cmdline->input.offset > 0
			&& !ft_isspace(cmdline->input.buffer[cmdline->input.offset - 1])
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
			&& !ft_isspace(cmdline->input.buffer[cmdline->input.offset])
			&& handle_move_right(cmdline))
		continue ;
	while (cmdline->input.offset < cmdline->input.size
			&& ft_isspace(cmdline->input.buffer[cmdline->input.offset])
			&& handle_move_right(cmdline))
		continue ;
	return (1);
}
