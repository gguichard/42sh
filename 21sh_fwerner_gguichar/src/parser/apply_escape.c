/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_escape.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 08:57:59 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/30 08:33:03 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "str_cmd_inf.h"
#include "apply_escape.h"

/*
** Retourne 1 si le caractere actuel doit etre supprime pour echappement,
** retourne 0 sinon. Les differents "is_in" de str_cmd_inf sont maj pour prendre
** en compte le caractere a supprimer.
*/

static int		need_to_remove_cur_char(t_str_cmd_inf *str_cmd_inf)
{
	t_str_cmd_inf	str_cmd_old;

	str_cmd_old.is_in_quote = str_cmd_inf->is_in_quote;
	str_cmd_old.is_in_doublequote = str_cmd_inf->is_in_doublequote;
	str_cmd_old.is_in_var_bracket = str_cmd_inf->is_in_var_bracket;
	str_cmd_old.str = str_cmd_inf->str;
	str_cmd_old.pos = str_cmd_inf->pos;
	if (str_cmd_old.str[str_cmd_old.pos] == '\"')
	{
		scmd_move_to_next_char(str_cmd_inf);
		--(str_cmd_inf->pos);
		return (str_cmd_old.is_in_doublequote
				!= str_cmd_inf->is_in_doublequote);
	}
	else if (str_cmd_old.str[str_cmd_old.pos] == '\'')
	{
		scmd_move_to_next_char(str_cmd_inf);
		--(str_cmd_inf->pos);
		return (str_cmd_old.is_in_quote != str_cmd_inf->is_in_quote);
	}
	return (0);
}

static void		remove_cur_char_from_scmd(t_str_cmd_inf *str_cmd_inf
		, char *new_str, size_t *str_len, int is_for_backslash)
{
	int		cur_move;

	cur_move = (is_for_backslash
			&& str_cmd_inf->str[str_cmd_inf->pos + 1] == '\n' ? 2 : 1);
	ft_memmove(new_str + str_cmd_inf->pos
			, new_str + str_cmd_inf->pos + cur_move
			, *str_len + 1 - str_cmd_inf->pos - cur_move);
	*str_len -= cur_move;
	if (is_for_backslash && str_cmd_inf->str[str_cmd_inf->pos] != '\0')
		str_cmd_inf->pos += (cur_move == 2 ? 0 : 1);
}

char			*apply_escape(const char *str)
{
	t_str_cmd_inf	str_cmd_inf;
	char			*new_str;
	size_t			str_len;

	str_len = ft_strlen(str);
	if ((new_str = ft_strndup(str, str_len)) == NULL)
		return (NULL);
	scmd_init(&str_cmd_inf, new_str);
	while (str_cmd_inf.str[str_cmd_inf.pos] != '\0')
	{
		if (str_cmd_inf.str[str_cmd_inf.pos] == '\\'
				&& (scmd_cur_char_is_in_nothing(&str_cmd_inf)
					|| (str_cmd_inf.is_in_doublequote
						&& ft_strchr("$\"\\\n"
							, str_cmd_inf.str[str_cmd_inf.pos + 1]) != NULL)))
			remove_cur_char_from_scmd(&str_cmd_inf, new_str, &str_len, 1);
		else if (need_to_remove_cur_char(&str_cmd_inf))
			remove_cur_char_from_scmd(&str_cmd_inf, new_str, &str_len, 0);
		else
			++(str_cmd_inf.pos);
	}
	scmd_delete(str_cmd_inf.sub_var_bracket);
	return (new_str);
}

char			*apply_only_newline_escape(const char *str)
{
	t_str_cmd_inf	str_cmd_inf;
	char			*new_str;
	size_t			str_len;

	str_len = ft_strlen(str);
	if ((new_str = ft_strndup(str, str_len)) == NULL)
		return (NULL);
	scmd_init(&str_cmd_inf, new_str);
	while (str_cmd_inf.str[str_cmd_inf.pos] != '\0')
	{
		if (str_cmd_inf.str[str_cmd_inf.pos] == '\\'
				&& str_cmd_inf.str[str_cmd_inf.pos + 1] == '\n'
				&& !str_cmd_inf.is_in_quote
				&& !scmd_cur_char_is_escaped(&str_cmd_inf))
			remove_cur_char_from_scmd(&str_cmd_inf, new_str, &str_len, 1);
		else
			scmd_move_to_next_char(&str_cmd_inf);
	}
	scmd_delete(str_cmd_inf.sub_var_bracket);
	return (new_str);
}
