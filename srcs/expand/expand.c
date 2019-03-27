#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"

int			expand_var(char **str, t_alloc *alloc, const char *exp)
{
	if (ft_strncmp(exp, "${", 2) == 0)
		*str = get_expand_value(&(exp[2]), 1, alloc);
	else
		*str = get_expand_value(&(exp[1]), 0, alloc);
	return (1);
}

int			expand(char **input, t_alloc *alloc, size_t *pos)
{
	char		*str;
	const char	*exp;

	exp = NULL;
	str = NULL;
	exp = ft_strchr(*input + *pos, '$');
	expand_var(&str, alloc, *input + *pos);
	if ((*input)[*pos + 1] == '{' && str)
		insert_var_input(str, input, 1, *pos);
	else
		insert_var_input(str, input, 0, *pos);
	if (str)
	{
		*pos += ft_strlen(str);
		ft_memdel((void **)&str);
	}
	return (1);
}
