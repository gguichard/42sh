/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 13:05:28 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/29 13:09:20 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "vars.h"
#include "shell.h"
#include "str_cmd_inf.h"
#include "expand_vars.h"

int		str_good_replace(char **str, size_t pos, size_t len
		, const char *replacement)
{
	size_t	str_len;
	size_t	replacement_len;
	char	*new_str;

	str_len = ft_strlen(*str);
	replacement_len = ft_strlen(replacement);
	if ((new_str = (char*)malloc(str_len - len + replacement_len + 1)) == NULL)
		return (0);
	new_str[0] = '\0';
	ft_strncat(new_str, *str, pos);
	ft_strcat(new_str, replacement);
	ft_strcat(new_str, *str + pos + len);
	free(*str);
	*str = new_str;
	return (1);
}

size_t	get_var_name_len(const char *str)
{
	size_t	var_len;

	var_len = 0;
	if (*str == '?' || ft_isdigit(*str))
		var_len = 1;
	else
	{
		while (ft_isalnum(*str) || *str == '_')
		{
			++var_len;
			++str;
		}
	}
	return (var_len);
}

int		char_need_to_be_escaped(t_str_cmd_inf *scmd, char this_char)
{
	if (this_char == '\0')
		return (0);
	else if (scmd == NULL)
		return (ft_strchr(" $\"\\\'~|<>;", this_char) != NULL);
	else if (scmd->is_in_quote || scmd->is_in_var_bracket)
		return (0);
	else if (scmd->is_in_doublequote)
		return (ft_strchr("$\"\\", this_char) != NULL);
	else
		return (ft_strchr("$\"\\\'~|<>;", this_char) != NULL);
}

char	*escape_chars_in_var(t_str_cmd_inf *scmd, const char *str)
{
	size_t		num_of_chars_to_esc;
	const char	*cur_char;
	char		*new_str;
	size_t		idx;

	num_of_chars_to_esc = 0;
	cur_char = str;
	while (*cur_char != '\0')
	{
		num_of_chars_to_esc += char_need_to_be_escaped(scmd, *cur_char);
		++cur_char;
	}
	if ((new_str = (char*)malloc(sizeof(char)
					* (ft_strlen(str) + num_of_chars_to_esc + 1))) == NULL)
		return (NULL);
	idx = 0;
	while (*str != '\0')
	{
		if (char_need_to_be_escaped(scmd, *str))
			new_str[idx++] = '\\';
		new_str[idx++] = *str;
		++str;
	}
	new_str[idx] = '\0';
	return (new_str);
}

int		replace_var_by_value(t_str_cmd_inf *scmd, char **new_str
		, size_t var_len, t_shell *shell)
{
	int		is_in_brackets;
	int		err_ret;
	char	*var_name;
	char	*var_value;
	char	*escaped_var_value;

	is_in_brackets = (scmd->str[scmd->pos + 1] == '{');
	if ((var_name = ft_strsub(scmd->str, scmd->pos + (is_in_brackets ? 2 : 1)
					, var_len)) == NULL)
		return (0);
	var_value = get_shell_var(shell, var_name);
	if ((escaped_var_value = escape_chars_in_var(scmd, var_value)) == NULL)
	{
		free(var_value);
		free(var_name);
		return (0);
	}
	err_ret = str_good_replace(new_str, scmd->pos
			, var_len + (is_in_brackets ? 3 : 1), escaped_var_value);
	scmd->str = *new_str;
	scmd->pos += ft_strlen(escaped_var_value);
	free(escaped_var_value);
	free(var_value);
	free(var_name);
	return (err_ret);
}
