#include "libft.h"
#include "shell.h"
#include "parser_lexer.h"
#include "error.h"
#include "vars.h"

void	expand_home_shortcut(char **s, t_list *vars)
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
	}
}

void	replace_str(char **str, char *insert, int pos)
{
	char *begin;
	char *end;
	char *tmp;

	if (!*str)
		(!(*str = ft_strdup(insert))) ? ft_exit_malloc() : 0;
	else
	{
		begin = ft_strsub(*str, 0, pos);
		end = ft_strsub(*str, pos, ft_strlen(*str));
		free(*str);
		if (!(tmp = ft_strjoin(begin, insert)))
			ft_exit_malloc();
		if (!(*str = ft_strjoin(tmp, end)))
			ft_exit_malloc();
		free(tmp);
		free(begin);
		free(end);
	}
}
