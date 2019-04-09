/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inhibitor_remove_only_newline.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 17:44:01 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/09 15:12:39 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "str_cmd_inf.h"
#include "inhibitor.h"

static void	go_to_end_sub_cmd(const char *str, size_t *pos
		, t_str_cmd_inf *str_cmd)
{
	*pos += scmd_move_to_next_char(str_cmd);
	while (scmd_cur_char(str_cmd))
	{
		if (scmd_cur_char(str_cmd) == ')'
				&& !scmd_cur_char_is_escaped(str_cmd))
		{
			*pos += scmd_move_to_next_char(str_cmd);
			break ;
		}
		else if (scmd_cur_char(str_cmd) == '$' && str[*pos + 1] == '(')
			go_to_end_sub_cmd(str, pos, str_cmd);
		else
			*pos += scmd_move_to_next_char(str_cmd);
	}
}

static void	go_to_end_single_or_dbquote(size_t *pos, t_str_cmd_inf *str_cmd)
{
	while (scmd_cur_char(str_cmd)
			&& (str_cmd->is_in_quote || str_cmd->is_in_dbquote))
		*pos += scmd_move_to_next_char(str_cmd);
}

static void	move_useless_chars(char **str, size_t *pos, t_str_cmd_inf *str_cmd)
{
	if ((*str)[*pos + 1] == '\0')
		(*str)[*pos] = '\0';
	else
		ft_memmove(*str + *pos - 1, *str + *pos + 1, ft_strlen(*str + *pos));
	*pos -= 1;
	scmd_move_to_next_char(str_cmd);
}

int			remove_only_escaped_newline(char **str)
{
	t_str_cmd_inf	str_cmd;
	size_t			pos;

	if (!scmd_init(&str_cmd, *str))
		return (0);
	pos = 0;
	while (scmd_cur_char(&str_cmd))
	{
		if (str_cmd.is_in_quote || str_cmd.is_in_dbquote)
			go_to_end_single_or_dbquote(&pos, &str_cmd);
		else if ((*str)[pos] == '$' && (*str)[pos + 1] == '('
				&& !scmd_cur_char_is_escaped(&str_cmd))
			go_to_end_sub_cmd(*str, &pos, &str_cmd);
		else if (scmd_cur_char(&str_cmd) == '\n'
				&& scmd_cur_char_is_escaped(&str_cmd))
			move_useless_chars(str, &pos, &str_cmd);
		else
			pos += scmd_move_to_next_char(&str_cmd);
	}
	scmd_clean(&str_cmd);
	return (1);
}
