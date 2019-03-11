/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/15 10:52:35 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 09:52:52 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "input.h"

static size_t	get_col_offset(t_term *term, size_t *index_ptr)
{
	size_t	row;
	size_t	col;
	size_t	index;

	row = 0;
	col = term->offset % term->win_cols;
	index = 0;
	while (index < term->size && row < term->row)
	{
		if ((col + 1) % term->win_cols != 0
				&& (term->line)[index] != '\n')
			col++;
		else
		{
			col = 0;
			row++;
		}
		index++;
	}
	if (index_ptr != NULL)
		*index_ptr = index;
	return (col);
}

size_t			get_max_col(t_term *term)
{
	size_t	col;
	size_t	index;

	col = get_col_offset(term, &index);
	while (col < (term->win_cols - 1))
	{
		if ((term->line)[index] == '\0'
				|| (term->line)[index] == '\n')
			break ;
		col++;
		index++;
	}
	return (col);
}

size_t			get_total_rows(t_term *term)
{
	size_t	rows;
	size_t	col;
	size_t	index;

	rows = 1;
	col = term->offset % term->win_cols;
	index = 0;
	while (index < term->size)
	{
		if ((col + 1) % term->win_cols != 0
				&& (term->line)[index] != '\n')
			col++;
		else
		{
			col = 0;
			rows++;
		}
		index++;
	}
	return (rows);
}

void			update_cursor_data(t_term *term)
{
	size_t	col;
	size_t	index;

	col = get_col_offset(term, &index);
	term->cursor = index + (term->col - col);
}

void			update_pos_data(t_term *term)
{
	size_t	row;
	size_t	col;
	size_t	index;

	row = 0;
	col = term->offset % term->win_cols;
	index = 0;
	while (index < term->cursor)
	{
		if ((col + 1) % term->win_cols != 0
				&& (term->line)[index] != '\n')
			col++;
		else
		{
			col = 0;
			row++;
		}
		index++;
	}
	term->row = row;
	term->col = col;
	term->rows = get_total_rows(term);
}
