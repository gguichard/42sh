#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"

int			expand_var(char **str, t_alloc *alloc, const char *exp, size_t *len)
{
	if (ft_strncmp(exp, "${", 2) == 0)
	{
		if (!check_expand_syntax(exp + 2))
			return (error_expand(exp));
		*str = get_expand_value(exp + 2, 1, alloc, len);
	}
	else
		*str = get_expand_value(exp + 1, 0, alloc, len);
	return (1);
}

int			expand(char **input, t_alloc *alloc, size_t *pos)
{
	char		*str;
	const char	*exp;
	size_t		len;

	exp = NULL;
	str = NULL;
	len = 0;
	exp = ft_strchr(*input + *pos, '$');
	if (!expand_var(&str, alloc, *input + *pos, &len))
		return (0);
	if ((*input)[*pos + 1] == '{' && str)
		ft_strreplace_inside(input, *pos, len + 3, str);
	else if (!ft_isalnum(exp[1]) && exp[1] != '_' && exp[1] != '?'
			&& exp[1] != '!' && exp[1] != '\'' && exp[1] != '"' && exp[1] != '$')
	{
		*pos += 1;
		return (1);
	}
	else
		ft_strreplace_inside(input, *pos, len + 1, str);
	if (str)
	{
		*pos += ft_strlen(str);
		ft_memdel((void **)&str);
	}
	return (1);
}
