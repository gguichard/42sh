/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd_token.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 14:15:31 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/29 11:04:41 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "str_cmd_inf.h"
#include "token_inf.h"
#include "split_cmd_token.h"

static int				process_opt_add(t_list **token_lst
		, t_str_cmd_inf *str_cmd_inf, const char *token_start)
{
	t_str_cmd_inf	str_cmd_cpy;
	int				end_by_and;
	t_token_type	token_type;

	str_cmd_cpy.str = str_cmd_inf->str;
	str_cmd_cpy.pos = str_cmd_inf->pos;
	end_by_and = 0;
	token_type = get_token_for_opt_add(str_cmd_inf, token_start, &end_by_and);
	if (end_by_and)
		--(str_cmd_cpy.pos);
	if ((str_cmd_cpy.str + str_cmd_cpy.pos) != token_start
			&& !add_cur_token_to_lst(token_lst, &str_cmd_cpy
				, token_start, token_type))
		return (0);
	if (end_by_and)
		if (!add_token_to_lst(token_lst, "&", TK_NUM_OPT))
			return (0);
	return (1);
}

static int				process_after_ope(t_list **token_lst
		, t_str_cmd_inf *str_cmd_inf)
{
	const char		*token_start;
	t_str_cmd_inf	str_cmd_cpy;

	str_cmd_cpy.str = str_cmd_inf->str;
	str_cmd_cpy.pos = str_cmd_inf->pos + 1;
	if (str_cmd_cpy.str[str_cmd_cpy.pos] == '&')
	{
		token_start = (str_cmd_cpy.str + str_cmd_cpy.pos);
		++(str_cmd_cpy.pos);
		while (ft_isdigit(str_cmd_cpy.str[str_cmd_cpy.pos]))
			++(str_cmd_cpy.pos);
		if (str_cmd_cpy.str[str_cmd_cpy.pos] == '-')
			++(str_cmd_cpy.pos);
		if (!is_a_sep_char(str_cmd_cpy.str[str_cmd_cpy.pos])
				&& str_cmd_cpy.str[str_cmd_cpy.pos] != '\0'
				&& token_start[1] != '-')
			str_cmd_cpy.pos = token_start - str_cmd_cpy.str + 1;
		if (!add_cur_token_to_lst(token_lst, &str_cmd_cpy
					, token_start, TK_STR_OPT))
			return (0);
		str_cmd_inf->pos = str_cmd_cpy.pos - 1;
	}
	return (1);
}

static int				split_sep_char(t_list **token_lst
		, t_str_cmd_inf *str_cmd_inf, const char *token_start
		, int last_char_was_sep)
{
	t_token_type	token_type;

	if (!last_char_was_sep)
		if (!process_opt_add(token_lst, str_cmd_inf, token_start))
			return (0);
	token_type = TK_OPE;
	token_start = str_cmd_inf->str + str_cmd_inf->pos;
	if (str_cmd_inf->str[str_cmd_inf->pos] != '\0'
			&& ft_strchr(" \t\n", str_cmd_inf->str[str_cmd_inf->pos]) == NULL)
	{
		if (str_cmd_inf->str[str_cmd_inf->pos] == ';')
			token_type = TK_CMD_SEP;
		str_cmd_inf->pos += get_cur_token_len(token_start) - 1;
		if (!add_cur_token_to_lst(token_lst, str_cmd_inf
					, token_start, token_type))
			return (0);
	}
	if (token_start == (str_cmd_inf->str + str_cmd_inf->pos)
			&& (str_cmd_inf->str[str_cmd_inf->pos] == '>'
				|| str_cmd_inf->str[str_cmd_inf->pos] == '<'))
	{
		if (!process_after_ope(token_lst, str_cmd_inf))
			return (0);
	}
	return (1);
}

static t_token_type		split_at_pos(t_list **token_lst
		, t_str_cmd_inf *str_cmd_inf, const char **token_start
		, int *last_char_was_sep)
{
	t_token_type	token_type;

	if (scmd_cur_char_is_in_nothing(str_cmd_inf)
			&& is_a_sep_char(str_cmd_inf->str[str_cmd_inf->pos])
			&& !scmd_cur_char_is_escaped(str_cmd_inf))
	{
		if (!split_sep_char(token_lst, str_cmd_inf, *token_start
					, *last_char_was_sep))
			return (TK_NOTHING);
		token_type = TK_OPE;
		*last_char_was_sep = 1;
	}
	else
	{
		token_type = TK_WORD;
		if (*last_char_was_sep)
			*token_start = str_cmd_inf->str + str_cmd_inf->pos;
		*last_char_was_sep = 0;
	}
	return (token_type);
}

t_list					*split_cmd_token(t_str_cmd_inf *str_cmd_inf)
{
	int				last_char_was_sep;
	t_list			*token_lst;
	t_token_type	token_type;
	const char		*token_start;

	last_char_was_sep = 1;
	token_lst = NULL;
	token_type = TK_NOTHING;
	token_start = str_cmd_inf->str;
	while (str_cmd_inf->str[str_cmd_inf->pos] != '\0')
	{
		if ((token_type = split_at_pos(&token_lst, str_cmd_inf, &token_start
						, &last_char_was_sep)) == TK_NOTHING)
			return (ft_lstdel(&token_lst, del_token));
		scmd_move_to_next_char(str_cmd_inf);
	}
	if (token_type == TK_WORD && !add_cur_token_to_lst(&token_lst
				, str_cmd_inf, token_start, token_type))
		return (ft_lstdel(&token_lst, del_token));
	return (token_lst);
}
