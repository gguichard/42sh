/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_cmd_inf_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 11:51:55 by fwerner           #+#    #+#             */
/*   Updated: 2019/04/09 13:06:06 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "str_cmd_inf.h"

int				scmd_init(t_str_cmd_inf *str_cmd_inf, const char *str)
{
	ft_bzero(str_cmd_inf, sizeof(t_str_cmd_inf));
	if (str != NULL)
	{
		str_cmd_inf->str = ft_strdup(str);
		return (str_cmd_inf->str != NULL);
	}
	else
		return (1);
}

void			scmd_move_pos(t_str_cmd_inf *str_cmd_inf, int pos_move)
{
	while (str_cmd_inf != NULL)
	{
		str_cmd_inf->pos += pos_move;
		str_cmd_inf = str_cmd_inf->sub_str_cmd;
	}
}

t_str_cmd_inf	*scmd_get_last_depth(t_str_cmd_inf *str_cmd_inf)
{
	while (str_cmd_inf->sub_str_cmd != NULL)
		str_cmd_inf = str_cmd_inf->sub_str_cmd;
	return (str_cmd_inf);
}

int				scmd_cur_char_is_escaped(t_str_cmd_inf *str_cmd_inf)
{
	return (scmd_char_at_is_escaped(str_cmd_inf, str_cmd_inf->pos));
}

int				scmd_cur_char_can_be_spe(t_str_cmd_inf *str_cmd_inf)
{
	return (scmd_char_at_can_be_spe(str_cmd_inf, str_cmd_inf->pos));
}
