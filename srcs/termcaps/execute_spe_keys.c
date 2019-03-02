/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_spe_keys.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/24 12:25:49 by jocohen           #+#    #+#             */
/*   Updated: 2019/02/21 12:00:08 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	ctrl_arrow_move(t_buf *input, char k)
{
	if (k == 68)
		while (input->x)
		{
			cursor_movement(input, -1);
			if (input->x && !(ft_isalnum(input->s[input->x - 1]))
				&& ft_isalnum(input->s[input->x]))
				break ;
		}
	else if (k == 67)
		while (input->s[input->x])
		{
			cursor_movement(input, 1);
			if (input->s[input->x] && !(ft_isalnum(input->s[input->x - 1]))
				&& ft_isalnum(input->s[input->x]))
				break ;
		}
	else if (k == 65)
		vertical_cursor(input, -1);
	else if (k == 66)
		vertical_cursor(input, 1);
}

void	del_char(t_buf *input, int type, t_env **lst)
{
	if (!type)
	{
		if (!input->x)
			return ;
		cursor_movement(input, -1);
		put_term_rout("dc");
		ft_memmove(input->s + input->x, input->s + input->x
			+ lenbyte(input->s[input->x]), ft_strlen(input->s + input->x + 1)
				+ lenbyte(input->s[input->x]));
		reactualize_output(input, lst);
	}
	else if (input->s[input->x])
	{
		ft_memmove(input->s + input->x, input->s + input->x
			+ lenbyte(input->s[input->x]), ft_strlen(input->s + input->x + 1)
				+ lenbyte(input->s[input->x]));
		put_term_rout("dc");
		reactualize_output(input, lst);
	}
}

void	home_end_move(t_buf *input, char k)
{
	if (k == 72)
		while (input->x)
			cursor_movement(input, -1);
	else if (k == 70)
		while (input->s[input->x])
			cursor_movement(input, 1);
}

void	arrow_move(t_buf *input, char k, t_historic **history, t_env **lst)
{
	if (k == 68 && input->x)
		cursor_movement(input, -1);
	else if (k == 67 && input->s[input->x])
		cursor_movement(input, 1);
	else if (k == 65)
		historic_move(input, history, -1, lst);
	else if (k == 66)
		historic_move(input, history, 1, lst);
}

void	page_move(t_buf *input, char k, t_historic **history, t_env **lst)
{
	if (k == 53)
		historic_move(input, history, -1, lst);
	else if (k == 54)
		historic_move(input, history, 1, lst);
}
