/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   insert_mode_ac.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 09:39:10 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 10:20:45 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "shell.h"
#include "input.h"

static void			init_ac_format(t_ac_format *fmt, t_list *lst, t_term *term)
{
	fmt->elems = 0;
	fmt->col_width = 0;
	while (lst != NULL)
	{
		(fmt->elems)++;
		fmt->col_width = ft_max(ft_strlen((char *)lst->content)
				, fmt->col_width);
		lst = lst->next;
	}
	fmt->col_width += 2;
	fmt->max_col = term->win_cols / fmt->col_width;
	fmt->max_row = (int)ft_ceil(fmt->elems / (double)fmt->max_col);
	fmt->max_col = (int)ft_ceil(fmt->elems / (double)fmt->max_row);
}

static t_ac_format	*ac_get_format(t_list *lst, t_term *term)
{
	t_ac_format	*fmt;
	int			col;
	int			index;

	if ((fmt = (t_ac_format *)malloc(sizeof(t_ac_format))) == NULL)
		return (NULL);
	init_ac_format(fmt, lst, term);
	if (fmt->max_col <= 0 || fmt->max_row <= 0 || (fmt->cols = (t_list **)
				malloc(fmt->max_col * sizeof(t_list *))) == NULL)
	{
		free(fmt);
		return (NULL);
	}
	col = 0;
	index = 0;
	while (lst != NULL && col < fmt->max_col)
	{
		while (index++ != col * (fmt->max_row + 1))
			lst = lst->next;
		(fmt->cols)[col] = lst;
		col++;
	}
	return (fmt);
}

static void			ac_print_simple_list(t_list *lst)
{
	while (lst != NULL)
	{
		ft_dprintf(2, "%s\n", lst->content);
		lst = lst->next;
	}
}

void				ac_print_list(t_list *lst, t_term *term)
{
	t_ac_format	*fmt;
	int			row;
	int			col;

	if ((fmt = ac_get_format(lst, term)) == NULL)
	{
		ac_print_simple_list(lst);
		return ;
	}
	row = 0;
	while (row < fmt->max_row)
	{
		col = 0;
		while (col < fmt->max_col && (fmt->cols)[col] != NULL)
		{
			ft_dprintf(2, "%-*s", fmt->col_width, (fmt->cols)[col]->content);
			(fmt->cols)[col] = (fmt->cols)[col]->next;
			col++;
		}
		ft_putchar_fd('\n', 2);
		row++;
	}
	free(fmt->cols);
	free(fmt);
}
