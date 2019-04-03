#include "libft.h"
#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"
#include "error.h"

int		error_expand(const char *exp)
{
	ft_dprintf(2, "42sh: %s: bad substitution\n", exp);
	return (0);
}

int		check_expand_syntax(const char *str)
{
	int i;

	i = 0;
	if (str[i] == '}')
		return (0);
	if (ft_isdigit(str[i]))
	{
		while (ft_isdigit(str[i]))
			i += 1;
		if (str[i] != '}')
			return (0);
	}
	else if (str[i] == '!' || str[i] == '?' || str[i] == '$' || str[i] == '_')
	{
		if (str[i + 1] != '}')
			return (0);
	}
	else
	{
		while (ft_isalnum(str[i]) || str[i] == '!' || str[i] == '?'
				|| str[i] == '$' || str[i] == '_')
			i += 1;
		if (str[i] != '}')
			return (0);
	}
	return (1);
}

void	insert_var_input(char *str, char **input, int type, size_t start)
{
	size_t	i;
	size_t	len;

	i = start + 1;
	len = 0;
	if (type == 1)
		while ((*input)[i] != '}')
			i += 1;
	else if ((*input)[i] == '?' || (*input)[i] == '!' || (*input)[i] == '$')
		i += 1;
	else if (!ft_isdigit((*input)[i]))
	{
		while ((*input)[i] && (ft_isalnum((*input)[i]) == 1
				|| (*input)[i] == '_'))
			i += 1;
		i -= 1;
	}
	if (start != i)
		ft_insert(input, str, start, i);
	else
	{
		len = ft_strlen(*input + start);
		ft_memmove(*input + start - 1, *input + start, len);
		(*input)[start + len - 1] = '\0';
	}
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

int		do_expand(char ***array, t_alloc *alloc, size_t *pos_array,
		t_str_cmd_inf *str_cmd)
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
		if (len + save != *pos_array)
		{
			if (!(expand_var_to_tab(array, len, pos_array, save)))
				return (0);
		}
	}
	scmd_move_to_next_char(str_cmd);
	update_pos_index(str_cmd);
	return (1);
}
