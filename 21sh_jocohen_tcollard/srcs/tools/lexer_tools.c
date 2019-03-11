/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/01 11:49:57 by tcollard          #+#    #+#             */
/*   Updated: 2019/02/21 18:32:03 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

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
		ft_exit_malloc();
	if (!(*source = ft_strjoin(tmp, end)))
		ft_exit_malloc();
	ft_memdel((void **)&begin);
	ft_memdel((void **)&tmp);
	ft_memdel((void **)&end);
	ft_memdel((void **)&insert);
}

void	ft_delete_inside(char **source, int start, int len)
{
	char	*begin;
	char	*end;

	begin = ft_strsub(*source, 0, start);
	end = ft_strsub(*source, start + len, ft_strlen(&(*source)[len + start]));
	free(*source);
	if (!(*source = ft_strjoin(begin, end)))
		ft_exit_malloc();
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
