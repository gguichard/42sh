/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inhibitor.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 17:59:39 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/10 11:41:27 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"
#include "error.h"

int		inhib_in_db(t_str_cmd_inf *str_cmd, size_t *pos, char **array
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
			if (!expand(&(array[index]), alloc, pos))
				return (0);
			scmd_move_to_next_char(str_cmd);
			update_pos_index(str_cmd);
		}
		else
			*pos += scmd_move_to_next_char(str_cmd);
	remove_last_quote(str_cmd, array, pos, index);
	return (1);
}

int		do_inhib(t_str_cmd_inf *str_cmd, char ***array, size_t *pos_array
		, t_alloc *alloc)
{
	while (scmd_cur_char(str_cmd))
	{
		if (str_cmd->is_in_quote || str_cmd->is_in_dbquote)
		{
			if (!(inhib_expand_in_quote(str_cmd, *array, pos_array, alloc)))
				return (error_inhib_expand(str_cmd, *array));
		}
		else if (scmd_cur_char_is_escaped(str_cmd))
			remove_escaped_char(str_cmd, &((*array)[get_pos_in_array(*array)])
					, pos_array, 1);
		else if (scmd_cur_char(str_cmd) == '$')
		{
			if (!do_expand(array, alloc, pos_array, str_cmd))
				return (error_inhib_expand(str_cmd, *array));
		}
		else
			*pos_array += scmd_move_to_next_char(str_cmd);
	}
	remove_last_char(str_cmd, pos_array, &((*array)[get_pos_in_array(*array)]));
	return (1);
}

static void	clean_empty_line_tab(char ***array)
{
	size_t	i;

	i = 0;
	if (ft_strtab_count(*array) == 1)
		return ;
	if (ft_strequ((*array)[i], "") == 1)
		delete_line_tab(array, i);
	if (*array)
	{
		i = ft_strtab_count(*array) - 1;
		if (ft_strequ((*array)[i], "") == 1)
			delete_line_tab(array, i);
	}
}

char	**inhib_expand_str(const char *str, t_alloc *alloc)
{
	size_t			pos_array;
	t_str_cmd_inf	*str_cmd;
	char			**array;

	pos_array = 0;
	str_cmd = NULL;
	array = NULL;
	if (!str)
		return (NULL);
	if (!(initialize_inhib_expand(&str_cmd, &array, str)))
	{
		error_inhib_expand(str_cmd, array);
		return (NULL);
	}
	check_expand_home(&(array[0]), alloc->vars, str_cmd, &pos_array);
	if (!do_inhib(str_cmd, &array, &pos_array, alloc))
		return (NULL);
	scmd_clean(str_cmd);
	free(str_cmd);
	if (!array && (array = (char **)malloc((sizeof(char *) * 1))))
		array[0] = NULL;
	else if (array)
		clean_empty_line_tab(&array);
	return (array);
}

int		inhib_expand_tab(t_ast *elem, t_alloc *alloc)
{
	int		i;
	char	**new_array;

	i = 0;
	while (elem->input && elem->input[i])
	{
		if (!(new_array = inhib_expand_str(elem->input[i], alloc)))
			return (0);
		if (new_array[0])
			create_new_input(elem, &i, new_array);
		else
		{
			ft_strtab_free(new_array);
			delete_line_tab(&(elem->input), i);
		}
	}
	if (!(elem->input) || !(elem->input[0]))
		return (0);
	return (1);
}
