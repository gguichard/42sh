/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 14:39:11 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/22 14:15:57 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "shell.h"
#include "vars.h"

t_list		*parse_env(char **environ)
{
	int		index;
	t_list	*lst;
	char	*def;
	char	*sep;
	int		ret;

	index = 0;
	lst = NULL;
	ret = 1;
	while (environ[index] != NULL)
	{
		if (!(def = ft_strdup(environ[index]))
				|| (sep = ft_strchr(def, '=')) == NULL)
			ret = 0;
		else
		{
			*sep = '\0';
			ret = create_var(&lst, def, sep + 1);
		}
		free(def);
		if (!ret)
			return (ft_lstdel(&lst, free_var));
		index++;
	}
	return (lst);
}

static char	*env_key_equal_value(t_var *var)
{
	size_t	key_len;
	size_t	val_len;
	char	*pair;

	key_len = ft_strlen(var->key);
	val_len = ft_strlen(var->value);
	pair = (char *)malloc((key_len + val_len + 2) * sizeof(char));
	if (pair == NULL)
		return (NULL);
	ft_memcpy(pair, var->key, key_len);
	ft_memcpy(pair + key_len + 1, var->value, val_len + 1);
	pair[key_len] = '=';
	return (pair);
}

char		**get_environ_from_list(t_list *env)
{
	size_t	size;
	char	**environ;
	size_t	index;
	t_var	*var;

	size = ft_lstsize(env);
	environ = (char **)malloc((size + 1) * sizeof(char *));
	if (environ != NULL)
	{
		index = 0;
		while (index < size && env != NULL)
		{
			var = (t_var *)env->content;
			environ[index] = env_key_equal_value(var);
			if (environ[index] == NULL)
				return (ft_strtab_free(environ));
			env = env->next;
			index++;
		}
		environ[index] = NULL;
	}
	return (environ);
}
