#include "shell.h"
#include "parser_lexer.h"

int		ft_isquote(char c)
{
	if (c == '\'' || c == '"' || c == '`')
		return (1);
	return (0);
}

int		ft_isoperator(char c)
{
	if (c == '>' || c == '<' || c == '|' || c == '&')
		return (1);
	return (0);
}

void	ft_insert(char **source, char *insert, int pos1, int pos2)
{
	char	*begin;
	char	*end;
	char	*tmp;

	begin = ft_strsub(*source, 0, pos1);
	end = ft_strsub(*source, pos2 + 1, (ft_strlen(&((*source)[pos2]) - 1)));
	ft_memdel((void **)source);
	if (!(tmp = ft_strjoin(begin, insert)))
		return ;
	if (!(*source = ft_strjoin(tmp, end)))
		return ;
	ft_memdel((void **)&begin);
	ft_memdel((void **)&tmp);
	ft_memdel((void **)&end);
}

void	ft_delete_inside(char **source, int start, int len)
{
	char	*begin;
	char	*end;

	begin = ft_strsub(*source, 0, start);
	end = ft_strsub(*source, start + len, ft_strlen(&(*source)[len + start]));
	free(*source);
	if (!(*source = ft_strjoin(begin, end)))
		return ;
	free(begin);
	free(end);
}

int		ft_str_is_empty(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (1);
	while (s[i])
	{
		if (ft_isspace(s[i]) == 0)
			return (0);
		i += 1;
	}
	return (1);
}
