/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 23:41:38 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/09 17:55:14 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "shell.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"

int		error_expand(const char *exp)
{
	ft_dprintf(STDOUT_FILENO, "42sh: %s: bad substitution\n", exp);
	return (0);
}

int		check_expand_syntax(const char *str)
{
	int i;

	i = 0;
	if (str[i] == '}')
		return (0);
	if (ft_isdigit(str[i]))
		while (ft_isdigit(str[i]))
			i += 1;
	else if (str[i] == '!' || str[i] == '?' || str[i] == '$')
		return ((str[i + 1] != '}') ? 0 : 1);
	else
		while (ft_isalnum(str[i]) || str[i] == '_')
			i += 1;
	return ((str[i] != '}') ? 0 : 1);
}

char	*get_expand_value(const char *exp, int type, t_alloc *alloc, size_t *i)
{
	char	*str;
	char	*value;

	*i = 0;
	str = NULL;
	if (type == 1)
		while (exp[*i] && exp[*i] != '}')
			*i += 1;
	else if (ft_isdigit(exp[*i]) || exp[*i] == '?' || exp[*i] == '!'
			|| exp[*i] == '$')
		*i += 1;
	else
		while (exp[*i] && (ft_isalnum(exp[*i]) == 1 || exp[*i] == '_'))
			*i += 1;
	str = ft_strndup(exp, *i);
	value = get_var_for_expand(alloc, str);
	ft_memdel((void **)&str);
	return (value);
}

int		do_expand(char ***array, t_alloc *alloc, size_t *pos_array
		, t_str_cmd_inf *str_cmd)
{
	size_t	save;
	size_t	index;
	size_t	len;

	len = 0;
	save = *pos_array;
	index = get_pos_in_array(*array);
	if (!expand(&((*array)[index]), alloc, pos_array))
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
