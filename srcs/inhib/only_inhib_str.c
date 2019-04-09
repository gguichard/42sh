/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   only_inhib_str.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 09:22:28 by fwerner           #+#    #+#             */
/*   Updated: 2019/04/09 12:44:14 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "str_cmd_inf.h"

static void		set_last_depth_with_level(t_str_cmd_inf *scmd
		, t_str_cmd_inf *result, int *level)
{
	*level = 0;
	while (scmd->sub_str_cmd != NULL)
	{
		++(*level);
		scmd = scmd->sub_str_cmd;
	}
	ft_memcpy(result, scmd, sizeof(t_str_cmd_inf));
}

static int		move_and_remove_englobing_chars(t_str_cmd_inf *scmd)
{
	int				old_level;
	t_str_cmd_inf	old_scmd;
	int				new_level;
	t_str_cmd_inf	new_scmd;

	set_last_depth_with_level(scmd, &old_scmd, &old_level);
	if (!scmd_move_to_next_char(scmd))
		return (0);
	set_last_depth_with_level(scmd, &new_scmd, &new_level);
	if (old_level == new_level)
	{
		if (old_scmd.is_in_quote != new_scmd.is_in_quote
				|| old_scmd.is_in_dbquote != new_scmd.is_in_dbquote)
		{
			ft_memmove(scmd->str + scmd->pos - 1
					, scmd_cur_str(scmd)
					, ft_strlen(scmd_cur_str(scmd)) + 1);
			scmd_move_pos(scmd, -1);
		}
	}
	return (1);
}

static void		change_all_str_scmd(t_str_cmd_inf *scmd, char *new_str)
{
	while (scmd != NULL)
	{
		scmd->str = new_str;
		scmd = scmd->sub_str_cmd;
	}
}

char			*only_inhib_str(const char *str)
{
	t_str_cmd_inf	scmd;
	int				del_cur_char;
	char			*cur_char_ptr;
	char			*new_str;

	if (str == NULL || !scmd_init(&scmd, str))
		return (NULL);
	new_str = scmd.str;
	while (1)
	{
		if (scmd_cur_char_can_be_spe(&scmd)
				&& scmd_cur_char_is_escaped(&scmd))
		{
			del_cur_char = (scmd_cur_char(&scmd) == '\n');
			cur_char_ptr = scmd.str + scmd.pos;
			ft_memmove(cur_char_ptr - 1, cur_char_ptr + del_cur_char
					, ft_strlen(cur_char_ptr + del_cur_char) + 1);
			change_all_str_scmd(&scmd, cur_char_ptr - del_cur_char);
			scmd_move_pos(&scmd, -(scmd.pos));
		}
		else if (!move_and_remove_englobing_chars(&scmd))
			break ;
	}
	scmd_delete_sub_str(scmd.sub_str_cmd);
	return (new_str);
}
