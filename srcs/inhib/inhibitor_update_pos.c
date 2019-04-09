/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inhibitor_update_pos.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 18:15:39 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/08 21:01:47 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "str_cmd_inf.h"

static void	update_pos_bracket(t_str_cmd_inf *str_cmd)
{
	int	was_in_subcmd;

	if (scmd_cur_char(str_cmd) == '{')
	{
		while (scmd_cur_char(str_cmd) && scmd_cur_char(str_cmd) != '}')
			scmd_move_to_next_char(str_cmd);
		scmd_move_to_next_char(str_cmd);
	}
	else if (scmd_cur_char(str_cmd) == '(')
	{
		while (scmd_cur_char(str_cmd))
		{
			was_in_subcmd = (str_cmd->sub_str_cmd != NULL
				&& str_cmd->sub_str_cmd->cur_str_cmd_type == SCMD_TYPE_SUBCMD);
			scmd_move_to_next_char(str_cmd);
			if (was_in_subcmd
					&& (str_cmd->sub_str_cmd == NULL
						|| str_cmd->sub_str_cmd->cur_str_cmd_type
						!= SCMD_TYPE_SUBCMD))
				break ;
		}
	}
}

void		update_pos_index(t_str_cmd_inf *str_cmd)
{
	if (scmd_cur_char(str_cmd) == '?' || scmd_cur_char(str_cmd) == '!'
			|| scmd_cur_char(str_cmd) == '$' || scmd_cur_char(str_cmd) == ')')
	{
		scmd_move_to_next_char(str_cmd);
		return ;
	}
	if (scmd_cur_char(str_cmd) != '{' && scmd_cur_char(str_cmd) != '(')
	{
		while (scmd_cur_is_of(str_cmd, DBQUOTE_SPE_CHAR) == 0
				&& scmd_cur_char(str_cmd)
				&& (ft_isalnum(scmd_cur_char(str_cmd))
					|| scmd_cur_char(str_cmd) == '_'))
			scmd_move_to_next_char(str_cmd);
	}
	else
		update_pos_bracket(str_cmd);
}
