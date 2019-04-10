/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inhibitor_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 17:49:12 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/09 21:32:44 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"

int		initialize_inhib_expand(t_str_cmd_inf **str_cmd, char ***array
		, const char *str)
{
	if (!(*array = ft_memalloc(sizeof(char *) * 2)))
		return (0);
	if (!(*str_cmd = (t_str_cmd_inf*)malloc(sizeof(t_str_cmd_inf))))
		return (0);
	if (!scmd_init(*str_cmd, str))
		return (0);
	if (!((*array)[0] = ft_strdup(str)))
		return (0);
	return (1);
}

int		go_to_end_quote(t_str_cmd_inf *str_cmd, char **array, size_t *pos)
{
	size_t	i;

	i = get_pos_in_array(array);
	remove_escaped_char(str_cmd, &(array[i]), pos, 0);
	*pos -= 1;
	while (scmd_cur_char(str_cmd) && str_cmd->is_in_quote == 1)
		*pos += scmd_move_to_next_char(str_cmd);
	str_cmd->pos -= 1;
	if (scmd_cur_char(str_cmd) == '\''
			|| (scmd_cur_char(str_cmd) == '\\' && !str_cmd->is_in_quote
				&& !str_cmd->is_in_dbquote))
	{
		str_cmd->pos += 1;
		remove_escaped_char(str_cmd, &(array[i]), pos, 0);
		*pos -= 1;
	}
	else
	{
		str_cmd->pos += 1;
		*pos += 1;
	}
	return (1);
}

int		inhib_expand_in_quote(t_str_cmd_inf *str_cmd, char **array
		, size_t *pos, t_alloc *alloc)
{
	if (str_cmd->is_in_quote)
		return (go_to_end_quote(str_cmd, array, pos));
	else if (str_cmd->is_in_dbquote)
		return (inhib_in_db(str_cmd, pos, array, alloc));
	return (1);
}

void	delete_line_tab(char ***array, int i)
{
	size_t	len;

	len = ft_strtab_count(*array);
	if (len == 1)
		*array = ft_strtab_free(*array);
	else if (len - 1 == (size_t)i)
		ft_strdel(&((*array)[i]));
	else
	{
		ft_strdel(&((*array)[i]));
		i += 1;
		while ((*array)[i])
		{
			(*array)[i - 1] = (*array)[i];
			i += 1;
		}
		(*array)[i - 1] = (*array)[i];
	}
}
