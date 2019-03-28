/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_autocomplete.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 10:56:11 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/28 11:05:56 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "cmdline.h"

static void	init_ac_format(t_ac_format *fmt, t_list *lst, t_cmdline *cmdline)
{
	fmt->elems = 0;
	fmt->col_width = 0;
	while (lst != NULL)
	{
		fmt->elems += 1;
		fmt->col_width = ft_max(ft_strlen((const char *)lst->content)
				, fmt->col_width);
		lst = lst->next;
	}
	fmt->col_width += 2;
	fmt->max_col = cmdline->winsize.ws_col / ft_max(fmt->col_width, 1);
	fmt->max_row = (int)ft_ceil(fmt->elems / (double)fmt->max_col);
	fmt->max_col = (int)ft_ceil(fmt->elems / (double)fmt->max_row);
}

static int	ac_setup_format(t_ac_format *fmt, t_list *lst, t_cmdline *cmdline)
{
	int	col;
	int	index;

	init_ac_format(fmt, lst, cmdline);
	if (fmt->max_col <= 0 || fmt->max_row <= 0 || (fmt->cols = (t_list **)
				malloc(fmt->max_col * sizeof(t_list *))) == NULL)
		return (0);
	col = 0;
	index = 0;
	while (lst != NULL && col < fmt->max_col)
	{
		while (index++ != col * (fmt->max_row + 1))
			lst = lst->next;
		fmt->cols[col] = lst;
		col++;
	}
	return (1);
}

static void	ac_print_simple_list(t_list *lst)
{
	while (lst != NULL)
	{
		ft_printf("%s\n", lst->content);
		lst = lst->next;
	}
}

void		ac_print_list(t_list *lst, t_cmdline *cmdline)
{
	t_ac_format	fmt;
	int			row;
	int			col;

	if (!ac_setup_format(&fmt, lst, cmdline))
	{
		ac_print_simple_list(lst);
		return ;
	}
	row = 0;
	while (row < fmt.max_row)
	{
		col = 0;
		while (col < fmt.max_col && fmt.cols[col] != NULL)
		{
			ft_printf("%-*s", fmt.col_width, fmt.cols[col]->content);
			fmt.cols[col] = fmt.cols[col]->next;
			col++;
		}
		write(STDOUT_FILENO, "\n", 1);
		row++;
	}
	free(fmt.cols);
}
