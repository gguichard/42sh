/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_movement.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/10 14:28:03 by jocohen           #+#    #+#             */
/*   Updated: 2019/02/21 17:17:28 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	move_front_or_back(t_historic **lst, int direction)
{
	t_historic	*tmp;

	tmp = (*lst);
	if (direction == -1 && tmp->prev)
	{
		tmp = tmp->prev;
		while (tmp->modif && !(tmp->modif[0]) && tmp->next)
		{
			if (!tmp->prev)
				return ;
			tmp = tmp->prev;
		}
	}
	else if (direction == 1 && tmp->next)
	{
		tmp = tmp->next;
		while (tmp->modif && !(tmp->modif[0]) && tmp->next)
		{
			if (!tmp->next)
				return ;
			tmp = tmp->next;
		}
	}
	(*lst) = tmp;
}

void	install_new_line(t_historic **history, t_buf *input, t_env **env)
{
	t_historic	*tmp;

	tmp = (*history);
	check_over_buffer(input,
		(*history)->modif ? (*history)->modif : (*history)->origin);
	ft_strcpy(input->s, (tmp->modif) ? tmp->modif : tmp->origin);
	input->x = ft_strlen(input->s);
	delete_line_pos(input, env);
}

void	cmd_spe_search(t_historic **history, t_buf *input, int direction,
						t_env **lst)
{
	char			c;
	t_historic		*tmp;
	static char		*search_s;

	(search_s && check_prev_read(0) != -6) ? ft_memdel((void **)&search_s) : 0;
	if (!search_s)
	{
		c = input->s[input->x];
		input->s[input->x] = 0;
		(!(search_s = ft_strdup(input->s))) ? ft_exit_malloc() : 0;
		input->s[input->x] = c;
	}
	tmp = *history;
	while (tmp)
	{
		(direction == -1) ? tmp = tmp->prev : 0;
		(direction != -1) ? tmp = tmp->next : 0;
		if (tmp && search_cmd_common((tmp->modif) ? tmp->modif : tmp->origin,
			search_s))
			break ;
	}
	if (!tmp)
		return ;
	(*history) = tmp;
	install_new_line(history, input, lst);
}

void	historic_move(t_buf *input, t_historic **history, int direction,
							t_env **lst)
{
	static int		in_zero;

	check_prev_read(-6);
	if (check_prev_read(0) != -6 && in_zero)
		in_zero = 0;
	if (ft_strcmp(input->s, (*history)->origin))
	{
		ft_memdel((void **)&((*history)->modif));
		if (!((*history)->modif = ft_strdup(input->s)))
			ft_exit_malloc();
	}
	if (input->x && !in_zero)
	{
		in_zero = 0;
		cmd_spe_search(history, input, direction, lst);
		return ;
	}
	in_zero = 1;
	move_front_or_back(history, direction);
	install_new_line(history, input, lst);
}
