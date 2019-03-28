#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"

int		error_expand(const char *exp)
{
	ft_dprintf(2, "42sh: %s: bad substitution\n", exp);
	return (0);
}

int		check_expand_syntax(const char *str)
{
	int i;

	i = 0;
	if (ft_isdigit(str[i]))
	{
		while (ft_isdigit(str[i]))
			i += 1;
		if (str[i] != '}')
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
	if (!(str = ft_strndup(exp, *i)))
		ft_exit_malloc();
	value = get_var_for_expand(alloc, str);
	free(str);
	return (value);
}
