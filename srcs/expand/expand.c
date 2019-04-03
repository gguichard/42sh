#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"

static int	is_special_char(char c)
{
	if (c == '_' || c == '?' || c == '!' || c == '\'' || c == '"' || c == '$')
		return (1);
	return (0);
}

int			expand_var(char **str, t_alloc *alloc, const char *exp,
		size_t *len)
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
	exp = ft_strchr(*input + *pos, '$');
	if (!expand_var(&str, alloc, *input + *pos, &len))
		return (0);
	if (str && (*input)[*pos + 1] == '{' && str)
		ft_strreplace_inside(input, *pos, len + 3, str);
	else if (str && !ft_isalnum(exp[1]) && is_special_char(exp[1]) == 0)
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
