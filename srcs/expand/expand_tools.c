lst_var#include "shell.h"
#include "parser_lexer.h"

int	error_expand(char *exp)
{
	ft_dprintf(2, "42sh: %s: bad substitution\n", err);
	return (0);
}

int	check_expand_simple(char *str)
{
	if (ft_isalpha(str[0]) == 0 && str[0] != '_')
		return (0);
	return (1);
}


int	check_expand_bracket(char *str)
{
	int	i;

	i = 1;
	if (ft_isalpha(str[i]) == 0 && str[i] != '_')
		return (0);
	while (str[i])
	{
		if (ft_isalnum(str[i]) == 0 && str[i] != '-' && str[i] != '}')
			return (0);
		else if (str[i] == '}')
			break ;
		i += 1;
	}
	return (1);
}

char		*get_expand_value(t_var *lst_var, char *exp, int type)
{
	int		i;
	char	*str;

	i = 0;
	str = NULL;
	if (type == 1)
		while (exp[i] && exp[i] != '}')
			i += 1
	else
		while (exp[i] && ft_isspace(str[i]) == 0)
			i += 1;
	if (!(str = ft_strndup(exp, i)))
		ft_exit_malloc();
	while (lst_var)
	{
		if (ft_strcmp(lst_var->key, str) == 0)
			break ;
		else
			lst_var = lst_var->next;
	}
	free(str);
	return ((lst_var) ? lst_var->value : "");
}
