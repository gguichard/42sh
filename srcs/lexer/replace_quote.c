#include "libft.h"
#include "shell.h"
#include "parser_lexer.h"
#include "error.h"
#include "vars.h"

int	expand_home_shortcut(char **s, t_list *vars)
{
	char	*dir;

	dir = 0;
	if ((*s)[0] == '~' && ((*s)[1] == '/' || (*s)[1] == '\0'))
	{
		dir = ft_strjoin(get_var_value(vars, "HOME")
				, ((*s)[1] == '/') ? (*s) + 1 : 0);
	}
	else if (ft_strnequ(*s, "~-", 2) && ((*s)[2] == '/' || (*s)[2] == '\0'))
	{
		dir = ft_strjoin(get_var_value(vars, "OLDPWD")
				, ((*s)[2] == '/') ? (*s) + 2 : 0);
	}
	else if (ft_strnequ("~+", *s, 2) && ((*s)[2] == '/' || (*s)[2] == '\0'))
	{
		dir = ft_strjoin(get_var_value(vars, "PWD")
				, ((*s)[2] == '/') ? (*s) + 2 : 0);
	}
	if (dir)
	{
		ft_memdel((void **)&(*s));
		(*s) = dir;
		return (1);
	}
	return (0);
}
