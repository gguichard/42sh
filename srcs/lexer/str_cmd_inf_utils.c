/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_cmd_inf_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 11:51:55 by fwerner           #+#    #+#             */
/*   Updated: 2019/04/08 11:51:57 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "str_cmd_inf.h"

int		scmd_init(t_str_cmd_inf *str_cmd_inf, const char *str)
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

void	scmd_delete_sub_str(t_str_cmd_inf *str_cmd_inf)
{
	if (str_cmd_inf != NULL)
	{
		scmd_delete_sub_str(str_cmd_inf->sub_str_cmd);
		free(str_cmd_inf);
	}
}

void	scmd_clean(t_str_cmd_inf *str_cmd_inf)
{
	if (str_cmd_inf != NULL)
	{
		scmd_delete_sub_str(str_cmd_inf->sub_str_cmd);
		free(str_cmd_inf->str);
		scmd_init(str_cmd_inf, NULL);
	}
}

int		scmd_reset(t_str_cmd_inf *str_cmd_inf, const char *new_str)
{
	t_str_cmd_inf	new_scmd;

	if (scmd_init(&new_scmd, new_str))
	{
		scmd_clean(str_cmd_inf);
		ft_memcpy(str_cmd_inf, &new_scmd, sizeof(t_str_cmd_inf));
		return (1);
	}
	else
		return (0);
}

int		scmd_cur_char_is_escaped(t_str_cmd_inf *str_cmd_inf)
{
	return (scmd_char_at_is_escaped(str_cmd_inf, str_cmd_inf->pos));
}
