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
	ft_printf("EXP = |%s|\n", str);
	if (ft_isalpha(str[0]) == 0 && str[0] != '_')
		return (0);
	return (1);
}

void	insert_var_input(char *str, char **input, int type)
{
	int		start;
	int		end;

	start = 0;
	end = 0;
	while ((*input)[start] != '$')
		start += 1;
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

char	*get_expand_value(t_var *lst_var, const char *exp, int type)
{
	int		i;
	char	*str;

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
