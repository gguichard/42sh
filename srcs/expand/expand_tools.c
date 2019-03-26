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
	int		end;

	end = start + 1;
	if (type == 1)
		while ((*input)[end] != '}')
			end += 1;
	else
	{
		while ((*input)[end] && (ft_isalnum((*input)[end]) == 1
				|| (*input)[end] == '_'))
			end += 1;
		end -= 1;
	}
	ft_insert(input, str, start, end);
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
	else
		while (exp[i] && (ft_isalnum(exp[i]) == 1 || exp[i] == '_'))
			i += 1;
	if (!(str = ft_strndup(exp, i)))
		ft_exit_malloc();
	value = get_var_for_expand(alloc, str);
	free(str);
	return (value);
}
