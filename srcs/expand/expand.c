#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"

int			expand_var(char **str, t_alloc *alloc, const char *exp)
{
	if (ft_strncmp(exp, "${", 2) == 0)
	{
		if (!check_expand_syntax(exp + 2))
			return (error_expand(exp));
		*str = get_expand_value(exp + 2, 1, alloc);
	}
	else
		*str = get_expand_value(exp + 1, 0, alloc);
	return (1);
}

int			expand(char **input, t_alloc *alloc, size_t *pos)
{
	char		*str;
	const char	*exp;

	exp = NULL;
	str = NULL;
	exp = ft_strchr(*input + *pos, '$');
	if (!expand_var(&str, alloc, *input + *pos))
		return (0);
	if ((*input)[*pos + 1] == '{' && str)
		insert_var_input(str, input, 1, *pos);
	else if (!ft_isalnum(exp[1]) && exp[1] != '_' && exp[1] != '?'
			&& exp[1] != '!' && exp[1] != '\'' && exp[1] != '"')
	{
		*pos += 1;
		return (1);
	}
	else
		insert_var_input(str, input, 0, *pos);
	if (str)
	{
		*pos += ft_strlen(str);
		ft_memdel((void **)&str);
	}
	return (1);
}
