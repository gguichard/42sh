/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_hooks2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/05 14:19:25 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 09:53:04 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"
#include "utils.h"

int			move_cursor_prev_word(t_shell *shell, t_term *term)
{
	if (term->cursor == 0)
		return (0);
	while ((term->line[term->cursor - 1] == ' '
				|| (term->line[term->cursor - 1] == '\n'))
			&& move_cursor_left(shell, term))
		continue ;
	while (term->cursor > 0
			&& term->line[term->cursor - 1] != ' '
			&& (term->line[term->cursor - 1] != '\n')
			&& move_cursor_left(shell, term))
		continue ;
	return (1);
}

int			move_cursor_next_word(t_shell *shell, t_term *term)
{
	if (term->cursor == term->size)
		return (0);
	while (term->line[term->cursor] != ' '
			&& (term->line[term->cursor] != '\n')
			&& move_cursor_right(shell, term))
		continue ;
	while (term->cursor < term->size
			&& (term->line[term->cursor] == ' '
				|| (term->line[term->cursor] == '\n'))
			&& move_cursor_right(shell, term))
		continue ;
	return (1);
}

static void	fix_outbound_col(t_term *term)
{
	size_t	max_col;
	size_t	real_off;

	max_col = get_max_col(term);
	real_off = term->offset % term->win_cols;
	if (term->row == 0 && term->col < real_off)
		term->col = real_off;
	else if (term->col > max_col)
		term->col = max_col;
	move_cursor_to_col(term);
	update_cursor_data(term);
}

int			move_cursor_up(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->row == 0)
		return (0);
	(term->row)--;
	tputs(tgetstr("up", NULL), 1, t_putchar);
	fix_outbound_col(term);
	return (1);
}

int			move_cursor_down(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->row + 1 == term->rows)
		return (0);
	(term->row)++;
	tputs(tgetstr("do", NULL), 1, t_putchar);
	fix_outbound_col(term);
	return (1);
}
