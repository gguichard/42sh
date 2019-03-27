#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"

int		error_expand(char *exp)
{
	ft_dprintf(2, "42sh: %s: bad substitution\n", exp);
	return (0);
}

int		check_expand_syntax(const char *str)
{
	if (ft_isalpha(str[0]) == 0 && str[0] != '_' && str[0] != '?')
		return (0);
	return (1);
}

void	insert_var_input(char *str, char **input, int type, size_t start)
{
	size_t	end;
	size_t	len;

	end = start + 1;
	len = 0;
	if (type == 1)
		while ((*input)[end] != '}')
			end += 1;
	else if (!ft_isdigit((*input)[end]))
	{
		while ((*input)[end] && (ft_isalnum((*input)[end]) == 1
				|| (*input)[end] == '_'))
			end += 1;
		end -= 1;
	}
	if (start != end)
		ft_insert(input, str, start, end);
	else
	{
		len = ft_strlen(*input + start);
		ft_memmove(*input + start - 1, *input + start, len);
		(*input)[start + len - 1] = '\0';
	}
}

char	*get_expand_value(const char *exp, int type, t_alloc *alloc)
{
	int		i;
	char	*str;
	char	*value;

	i = 0;
	str = NULL;
	if (type == 1)
		while (exp[i] && exp[i] != '}')
			i += 1;
	else if (ft_isdigit(exp[i]) || exp[i] == '?' || exp[i] == '!'
			|| exp[i] == '$')
		i += 1;
	else
		while (exp[i] && (ft_isalnum(exp[i]) == 1 || exp[i] == '_'))
			i += 1;
	if (!(str = ft_strndup(exp, i)))
		ft_exit_malloc();
	value = get_var_for_expand(alloc, str);
	free(str);
	return (value);
}
