/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_quote.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/01 11:26:07 by tcollard          #+#    #+#             */
/*   Updated: 2019/02/27 11:59:58 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static void	short_cut(char **s, t_env *lst_env)
{
	char	*dir;

	dir = 0;
	if ((*s)[0] == '~' && ((*s)[1] == '/' || (*s)[1] == '\0'))
	{
		dir = ft_strjoin(get_env_value(lst_env, "$HOME"),
			((*s)[1] == '/') ? (*s) + 1 : 0);
	}
	else if (ft_strncmp(*s, "~-", 2) == 0 && ((*s)[2] == '/'
	|| (*s)[2] == '\0'))
	{
		dir = ft_strjoin(get_env_value(lst_env, "$OLDPWD"),
			((*s)[2] == '/') ? (*s) + 2 : 0);
	}
	else if (ft_strncmp("~+", *s, 2) == 0 && ((*s)[2] == '/'
	|| (*s)[2] == '\0'))
	{
		dir = ft_strjoin(get_env_value(lst_env, "$PWD"),
			((*s)[2] == '/') ? (*s) + 2 : 0);
	}
	if (dir)
	{
		ft_memdel((void **)&(*s));
		(*s) = dir;
	}
}

void		replace_str(char **str, char *insert, int pos)
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

static int	replace_env_var(char **str, int i, t_env *lst_env)
{
	char	*key;
	char	*value;
	int		x;

	x = 1;
	value = NULL;
	while ((*str)[i + x] && x < 80 && ft_isalnum((*str)[i + x]) == 1)
		x += 1;
	if (x >= 80)
	{
		write(2, "21sh: env: error too long arguments\n", 36);
		return (-1);
	}
	else if ((*str)[x] == '?')
		replace_val_ret(str, i, x + 1);
	else
	{
		key = ft_strsub(*str, i, x);
		value = get_env_value(lst_env, key);
		ft_delete_inside(str, i, x);
		(value[0] != '\0') ? replace_str(str, value, i) : 0;
		free(key);
	}
	return (0);
}

int			remove_quote(char **s, int *i, t_env *lst_env, t_alloc **alloc)
{
	char	*sub;
	char	quote;
	int		save;
	int		x;

	x = 0;
	quote = (*s)[(*i)++];
	save = *i;
	while ((*s)[*i] && (*s)[*i] != quote)
		*i += 1;
	sub = (ft_isquote(quote) == 1) ? ft_strsub(*s, save, *i - save) : NULL;
	while (sub[x] && quote == '"')
		x += (sub[x] == '$') ? replace_env_var(&sub, x, lst_env) : 1;
	if (quote == '`')
	{
		if (!sub[0])
		{
			ft_memdel((void **)&sub);
			(!(sub = ft_strdup(""))) ? ft_exit_malloc() : 0;
		}
		else if (!(sub = ft_back_quote(sub, lst_env, alloc)))
			return (0);
	}
	(sub != NULL) ? ft_insert(s, sub, save - 1, *i) : 0;
	return (1);
}

int			convert_quote(char **s, t_env **lst_env, t_alloc **alloc)
{
	int		i;

	i = 0;
	short_cut(s, *lst_env);
	while (s && (*s)[i])
	{
		if ((*s)[i] == '$')
		{
			if (replace_env_var(s, i, *lst_env) == -1)
				return (-1);
		}
		else if (ft_isquote((*s)[i]) == 1)
		{
			if (!remove_quote(s, &i, *lst_env, alloc))
				return (-1);
			i -= 2;
		}
		else
			i += 1;
	}
	return (0);
}
