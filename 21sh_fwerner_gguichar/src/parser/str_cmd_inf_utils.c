/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_cmd_inf_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/28 16:44:02 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/28 16:46:39 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "str_cmd_inf.h"

void	scmd_init(t_str_cmd_inf *str_cmd_inf, const char *str)
{
	str_cmd_inf->is_in_quote = 0;
	str_cmd_inf->is_in_doublequote = 0;
	str_cmd_inf->is_in_var_bracket = 0;
	str_cmd_inf->pos = 0;
	str_cmd_inf->str = str;
	str_cmd_inf->sub_var_bracket = NULL;
}

void	scmd_delete(t_str_cmd_inf *str_cmd_inf)
{
	if (str_cmd_inf != NULL)
	{
		scmd_delete(str_cmd_inf->sub_var_bracket);
		free(str_cmd_inf);
	}
}

int		scmd_cur_char_is_escaped(t_str_cmd_inf *str_cmd_inf)
{
	return (scmd_char_at_is_escaped(str_cmd_inf, str_cmd_inf->pos));
}
