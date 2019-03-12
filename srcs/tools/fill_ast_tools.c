#include "shell.h"
#include "parser_lexer.h"

void	get_last_index_split(int *i, char *s, int *wd)
{
	char	c;

	c = '\0';
	*wd += 1;
	while (ft_isoperator(s[*i]) == 0)
	{
		if (ft_isquote(s[*i]) == 1)
		{
			c = s[*i];
			while (s[*i] != c)
				*i += 1;
		}
		*i += 1;
	}
}

int		get_last_index(int *i, char *s, int *save, char **input)
{
	char	c;

	c = '\0';
	while (ft_isoperator(s[*i]) == 0)
	{
		if (ft_isquote(s[*i]) == 1)
		{
			c = s[*i];
			*i += 1;
			while (s[*i] && s[*i] != c)
				*i += 1;
		}
		*i += 1;
	}
	*input = (c == '\0') ? ft_strsub(s, *save, *i - *save) :
	ft_strsub(s, *save, *i - *save + 3);
	*save = *i;
	return (1);
}

int		get_last_operator(int *i, char *s, int *save, char **input)
{
	while (ft_isoperator(s[*i]) == 1)
		*i += 1;
	*i += (s[*i] == '-') ? 1 : 0;
	*input = ft_strsub(s, *save, *i - *save);
	*save = *i;
	return (1);
}

int		get_last_digit(int *i, char *s, int *save, char **input)
{
	while (ft_isdigit(s[*i]) == 1)
		*i += 1;
	*input = ft_strsub(s, *save, *i - *save);
	*save = *i;
	return (1);
}

void	fill_input(char **s, int end, int start, t_ast *elem)
{
	int		i;
	t_ast	*tmp;
	int		len;

	i = 0;
	tmp = NULL;
	if (end == start)
		return ;
	if (elem->back && elem->back->type <= AGREG)
		len = 2;
	else
		len = end - start + 1;
	if (!(elem->input = (char**)malloc(sizeof(char*) * len)))
		ft_exit_malloc();
	while (start < end && i < len - 1)
	{
		if (!(elem->input[i] = ft_strdup(s[start])))
			ft_exit_malloc();
		start += 1;
		i += 1;
	}
	elem->input[i] = NULL;
	elem->type = CMD;
}
