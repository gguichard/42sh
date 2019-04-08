#include <stdlib.h>
#include "libft.h"
#include "str_cmd_inf.h"
#include "autocomplete.h"

static int		is_valid_var_char(char var_char, size_t idx)
{
	if (idx == 0)
	{
		return (ft_isalpha(var_char) || var_char == '_' || var_char == '\0');
	}
	else
	{
		return (ft_isalnum(var_char) || var_char == '_' || var_char == '\0');
	}
}

static void		update_last_var_inf(t_str_cmd_inf *scmd
		, const char **last_var_start, int *is_in_bracket)
{
	if (*last_var_start != NULL && !*is_in_bracket)
	{
		if (!is_valid_var_char(scmd_cur_char(scmd)
					, scmd_cur_str(scmd) - *last_var_start))
			*last_var_start = NULL;
	}
	else if (*last_var_start != NULL && *is_in_bracket)
	{
		if (scmd->sub_str_cmd == NULL
				|| scmd->sub_str_cmd->cur_str_cmd_type != SCMD_TYPE_VAR)
			*last_var_start = NULL;
	}
	if (*last_var_start == NULL && !scmd->is_in_quote
			&& scmd->sub_str_cmd == NULL && scmd_cur_char(scmd) == '$'
			&& !scmd_cur_char_is_escaped(scmd))
	{
		*last_var_start = scmd_cur_str(scmd) + 1;
		*is_in_bracket = 0;
	}
	else if (*last_var_start == NULL && scmd->sub_str_cmd != NULL
			&& scmd->sub_str_cmd->cur_str_cmd_type == SCMD_TYPE_VAR)
	{
		*last_var_start = scmd_cur_str(scmd);
		*is_in_bracket = 1;
	}
}

const char		*find_last_var_start(const char *str, int *is_in_bracket)
{
	t_str_cmd_inf	scmd;
	const char		*last_var_start;

	*is_in_bracket = 0;
	if (str == NULL || !scmd_init(&scmd, str))
		return (NULL);
	last_var_start = NULL;
	while (1)
	{
		update_last_var_inf(&scmd, &last_var_start, is_in_bracket);
		if (!scmd_move_to_next_char(&scmd))
			break ;
	}
	if (last_var_start != NULL)
		last_var_start = str + (last_var_start - scmd.str);
	scmd_clean(&scmd);
	return (last_var_start);
}

const char		*find_last_assign_start(const char *str)
{
	size_t		idx;
	const char	*last_assign;

	if (str == NULL)
		return (NULL);
	idx = 0;
	while (str[idx] != '\0')
	{
		if (!is_valid_var_char(str[idx], idx))
			break ;
		++idx;
	}
	if (str[idx] != '=')
		return (NULL);
	last_assign = str + idx + 1;
	while (str[idx] != '\0')
	{
		if (str[idx] == ':')
			last_assign = str + idx + 1;
		++idx;
	}
	return (last_assign);
}

const char		*find_last_home_user(const char *str)
{
	const char	*original_str;

	if (str == NULL)
		return (NULL);
	original_str = str;
	if (*str == '~')
	{
		++str;
		while (*str != '\0')
		{
			if (*str == '/')
				return (NULL);
			++str;
		}
		return (original_str + 1);
	}
	else
		return (NULL);
}
