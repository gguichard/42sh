/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inhibitor_remove.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 17:44:01 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/08 14:26:33 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"

void	remove_escaped_char(t_str_cmd_inf *str_cmd, char **input, size_t *pos
		, int next)
{
	size_t	len;

	if (*pos == 0)
		*pos = str_cmd->pos;
	if (str_cmd->pos == 0)
		*pos = 1;
	len = ft_strlen(*input + *pos);
	ft_memmove((void*)(*input + *pos - 1),
			(const void*)(*input + *pos), len);
	(*input)[*pos + len - 1] = '\0';
	if (scmd_cur_char_is_escaped(str_cmd) == 1
			&& scmd_cur_char(str_cmd) == '\n' && !str_cmd->is_in_quote)
	{
		len = ft_strlen(*input + *pos);
		ft_memmove((void*)(*input + *pos - 1),
				(const void*)(*input + *pos), len);
		(*input)[*pos + len - 1] = '\0';
		if (*pos > 0)
			*pos -= 1;
	}
	if (next == 1)
		scmd_move_to_next_char(str_cmd);
}

void	remove_last_char(t_str_cmd_inf *str_cmd, size_t *pos, char **input)
{
	if (*pos == 0)
		return ;
	str_cmd->pos -= 1;
	*pos -= 1;
	if (scmd_cur_char(str_cmd) == '\\' && !scmd_cur_char_is_escaped(str_cmd))
	{
		*pos += 1;
		remove_escaped_char(str_cmd, input, pos, 1);
		*pos -= 1;
	}
	else if ((scmd_cur_char(str_cmd) == '\'' && (*input)[*pos] == '\''
				&& str_cmd->is_in_quote) || (scmd_cur_char(str_cmd) == '"'
					&& (*input)[*pos] == '"' && str_cmd->is_in_dbquote))
	{
		*pos += 1;
		remove_escaped_char(str_cmd, input, pos, 1);
		*pos -= 1;
	}
	str_cmd->pos += 1;
}

void	remove_last_quote(t_str_cmd_inf *str_cmd, char **array, size_t *pos
		, size_t index)
{
	str_cmd->pos -= 1;
	if ((scmd_cur_char(str_cmd) == '\"'
				&& !scmd_cur_char_is_escaped(str_cmd))
			|| scmd_cur_char(str_cmd) == '\\')
	{
		str_cmd->pos += 1;
		remove_escaped_char(str_cmd, &(array[index]), pos, 0);
		*pos -= 1;
	}
	else
	{
		str_cmd->pos += 1;
		*pos += 1;
	}
}

int		remove_only_escaped_newline(char **str)
{
	t_str_cmd_inf	str_cmd;
	size_t			pos;

	pos = 0;
	if (!scmd_init(&str_cmd, *str))
		return (0);
	while (scmd_cur_char(&str_cmd))
	{
		if (str_cmd.is_in_quote || str_cmd.is_in_dbquote)
			while (scmd_cur_char(&str_cmd) && (str_cmd.is_in_quote
						|| str_cmd.is_in_dbquote))
				pos += scmd_move_to_next_char(&str_cmd);
		else if (scmd_cur_char(&str_cmd) == '\n'
				&& scmd_cur_char_is_escaped(&str_cmd))
		{
			ft_memmove((void *)(*str + pos - 1), (void *)(*str + pos + 1)
					, ft_strlen(*str + pos));
			(*str)[pos + ft_strlen(*str + pos)] = '\0';
			scmd_move_to_next_char(&str_cmd);
		}
		else
			pos += scmd_move_to_next_char(&str_cmd);
	}
	scmd_clean(&str_cmd);
	return (1);
}
