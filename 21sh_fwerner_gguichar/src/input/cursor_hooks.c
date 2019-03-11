/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/04 12:18:37 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 09:53:11 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"
#include "utils.h"

int	move_cursor_to_col(t_term *term)
{
	tputs(tgetstr("cr", NULL), 1, t_putchar);
	if (term->col > 0)
		tputs(tparm(tgetstr("RI", NULL), term->col), 1, t_putchar);
	return (1);
}

int	move_cursor_left(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->cursor == 0)
		return (0);
	(term->cursor)--;
	if (term->col > (term->offset % term->win_cols)
			|| (term->col > 0 && term->row > 0))
	{
		(term->col)--;
		tputs(tgetstr("le", NULL), 1, t_putchar);
	}
	else
	{
		(term->row)--;
		term->col = get_max_col(term);
		tputs(tgetstr("up", NULL), 1, t_putchar);
		move_cursor_to_col(term);
	}
	return (1);
}

int	move_cursor_right(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->cursor == term->size)
		return (0);
	if ((term->col + 1) < term->win_cols
			&& (term->line)[term->cursor] != '\n')
	{
		(term->col)++;
		tputs(tgetstr("nd", NULL), 1, t_putchar);
	}
	else
	{
		(term->row)++;
		term->col = 0;
		tputs(tgetstr("do", NULL), 1, t_putchar);
		move_cursor_to_col(term);
	}
	(term->cursor)++;
	return (1);
}

int	move_cursor_home(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->cursor == 0)
		return (0);
	term->cursor = 0;
	if (term->row > 0)
	{
		tputs(tparm(tgetstr("UP", NULL), term->row), 1, t_putchar);
		term->row = 0;
	}
	term->col = term->offset % term->win_cols;
	move_cursor_to_col(term);
	return (1);
}

int	move_cursor_end(t_shell *shell, t_term *term)
{
	(void)shell;
	if (term->cursor == term->size)
		return (0);
	term->cursor = term->size;
	if ((term->row + 1) < term->rows)
	{
		tputs(tparm(tgetstr("DO", NULL), term->rows - (term->row + 1))
				, 1, t_putchar);
		term->row = term->rows - 1;
	}
	term->col = get_max_col(term);
	move_cursor_to_col(term);
	return (1);
}
