/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inhib_autocomplete.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 15:01:41 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/10 17:26:46 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"
#include "error.h"

int		do_inhib_auto(t_str_cmd_inf *str_cmd, char ***array, size_t *pos_array
		, t_alloc *alloc)
{
	int	state;

	state = 0;
	while (scmd_cur_char(str_cmd))
		if (str_cmd->is_in_quote || str_cmd->is_in_dbquote)
		{
			if (!(inhib_expand_quote_auto(str_cmd, *array, pos_array, alloc)))
				return (error_inhib_expand(str_cmd, *array));
		}
		else if (scmd_cur_char_is_escaped(str_cmd))
			remove_escaped_char(str_cmd, &((*array)[get_pos_in_array(*array)])
					, pos_array, 1);
		else if (scmd_cur_char(str_cmd) == '$')
		{
			if (!(state = do_expand_auto(array, alloc, pos_array, str_cmd)))
				return (error_inhib_expand(str_cmd, *array));
		}
		else if (scmd_cur_char(str_cmd) == '=' && state == 0)
			state = check_expand_home_assign(
				&((*array)[get_pos_in_array(*array)])
				, alloc->vars, str_cmd, pos_array);
		else
			*pos_array += scmd_move_to_next_char(str_cmd);
	remove_last_char(str_cmd, pos_array, &((*array)[get_pos_in_array(*array)]));
	return (1);
}

int		do_expand_auto(char ***array, t_alloc *alloc, size_t *pos_array
		, t_str_cmd_inf *str_cmd)
{
	size_t	save;
	size_t	index;
	size_t	len;

	len = 0;
	save = *pos_array;
	index = get_pos_in_array(*array);
	if (!expand(&((*array)[index]), alloc, pos_array, 1))
		return (0);
	if (!str_cmd->is_in_quote && !str_cmd->is_in_dbquote && *pos_array != save)
	{
		while ((*array)[index][save + len]
				&& !ft_isspace((*array)[index][save + len])
				&& save + len <= *pos_array)
			len += 1;
		if (len + save != *pos_array
				&& !(expand_var_to_tab(array, len, pos_array, save)))
			return (0);
	}
	else if (!str_cmd->is_in_quote && !str_cmd->is_in_dbquote
			&& ft_strequ((*array)[index], "") == 1)
		delete_line_tab(array, index);
	scmd_move_to_next_char(str_cmd);
	update_pos_index(str_cmd);
	return (1);
}

int		inhib_in_db_auto(t_str_cmd_inf *str_cmd, size_t *pos, char **array
		, t_alloc *alloc)
{
	size_t	index;

	index = get_pos_in_array(array);
	remove_escaped_char(str_cmd, &(array[index]), pos, 0);
	*pos -= 1;
	while (scmd_cur_char(str_cmd) && str_cmd->is_in_dbquote)
		if (scmd_cur_char_is_escaped(str_cmd) == 1
				&& scmd_cur_is_of(str_cmd, DBQUOTE_SPE_CHAR) == 1)
			remove_escaped_char(str_cmd, &(array[index]), pos, 1);
		else if (scmd_cur_char(str_cmd) == '$')
		{
			if (!expand(&(array[index]), alloc, pos, 1))
				return (0);
			scmd_move_to_next_char(str_cmd);
			update_pos_index(str_cmd);
		}
		else
			*pos += scmd_move_to_next_char(str_cmd);
	remove_last_quote(str_cmd, array, pos, index);
	return (1);
}

int		inhib_expand_quote_auto(t_str_cmd_inf *str_cmd, char **array
		, size_t *pos, t_alloc *alloc)
{
	if (str_cmd->is_in_quote)
		return (go_to_end_quote(str_cmd, array, pos));
	else if (str_cmd->is_in_dbquote)
		return (inhib_in_db_auto(str_cmd, pos, array, alloc));
	return (1);
}
