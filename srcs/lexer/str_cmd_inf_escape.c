/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_cmd_inf_escape.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 11:51:49 by fwerner           #+#    #+#             */
/*   Updated: 2019/04/09 12:00:39 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "str_cmd_inf.h"

static int		is_dollared_spe_char(t_str_cmd_inf *str_cmd_inf, size_t at_pos
		, const char *spe_char_lst)
{
	int		is_a_spe_char;

	if (at_pos <= 0 || str_cmd_inf->str[at_pos - 1] != '$'
			|| scmd_char_at_is_escaped(str_cmd_inf, at_pos - 1))
		return (0);
	is_a_spe_char = 0;
	while (*spe_char_lst)
	{
		if (*spe_char_lst == str_cmd_inf->str[at_pos])
		{
			is_a_spe_char = 1;
			break ;
		}
		++spe_char_lst;
	}
	return (is_a_spe_char);
}

int				scmd_char_at_is_escaped(t_str_cmd_inf *str_cmd_inf
		, size_t at_pos)
{
	str_cmd_inf = scmd_get_last_depth(str_cmd_inf);
	if (str_cmd_inf->is_in_quote)
		return (str_cmd_inf->str[at_pos] != '\'');
	if (!scmd_char_at_can_be_spe(str_cmd_inf, at_pos))
		return (1);
	return (at_pos > 0 && str_cmd_inf->str[at_pos - 1] == '\\'
			&& !scmd_char_at_is_escaped(str_cmd_inf, at_pos - 1));
}

int				scmd_char_at_can_be_spe(t_str_cmd_inf *str_cmd_inf
		, size_t at_pos)
{
	str_cmd_inf = scmd_get_last_depth(str_cmd_inf);
	if (str_cmd_inf->is_in_quote)
		return (str_cmd_inf->str[at_pos] == '\'');
	else if (str_cmd_inf->is_in_dbquote)
	{
		return (is_dollared_spe_char(str_cmd_inf, at_pos, "{(")
				|| ft_strchr(DBQUOTE_SPE_CHAR, str_cmd_inf->str[at_pos])
				!= NULL);
	}
	else
		return (1);
}
