/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   del_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 16:06:26 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 13:14:23 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"
#include "utils.h"

static void	del_on_cursor(t_shell *shell, t_term *term)
{
	if (term->cursor < term->size)
		ft_memmove(term->line + term->cursor, term->line + term->cursor + 1
				, term->size - term->cursor);
	(term->size)--;
	go_to_prompt(term);
	print_input(shell, term);
}

int			handle_bs_key(t_shell *shell, t_term *term)
{
	if (term->visual_mode || !move_cursor_left(shell, term))
		return (0);
	del_on_cursor(shell, term);
	return (1);
}

int			handle_del_key(t_shell *shell, t_term *term)
{
	if (term->visual_mode || term->cursor == term->size)
		return (0);
	del_on_cursor(shell, term);
	return (1);
}
