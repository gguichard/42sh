/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 16:41:55 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/05 20:14:19 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "vars.h"

void		free_var(void *content, size_t content_size)
{
	t_var	*var;

	(void)content_size;
	var = (t_var *)content;
	free(var->key);
	free(var->value);
	free(var->tmp_value);
	free(var);
}

t_var		*get_var(t_list *lst, const char *key)
{
	t_var	*var;

	while (lst != NULL)
	{
		var = (t_var *)lst->content;
		if (ft_strequ(key, var->key))
			return (var);
		lst = lst->next;
	}
	return (NULL);
}

int			create_var(t_list **lst, const char *key, const char *value
		, int is_env)
{
	t_var	var;
	t_list	*elem;

	ft_memset(&var, 0, sizeof(t_var));
	var.key = ft_strdup(key);
	var.value = (value == NULL) ? NULL : ft_strdup(value);
	var.is_env = is_env;
	if (var.key != NULL && (var.value != NULL || value == NULL))
	{
		elem = ft_lstnew(&var, sizeof(t_var));
		if (elem != NULL)
		{
			insert_new_elem(lst, elem);
			return (1);
		}
	}
	free(var.key);
	free(var.value);
	return (0);
}

int			update_var(t_list **lst, const char *key, const char *value)
{
	t_var	*var;
	char	*dup;

	var = get_var(*lst, key);
	if (var != NULL)
	{
		if (value == NULL)
			dup = NULL;
		else if ((dup = ft_strdup(value)) == NULL)
			return (0);
		free(var->value);
		var->value = dup;
		return (1);
	}
	return (create_var(lst, key, value, 1));
}

int			unset_var(t_list **lst, const char *key)
{
	t_list	*prev;
	t_list	*cur;
	t_var	*var;

	prev = NULL;
	cur = *lst;
	while (cur != NULL)
	{
		var = (t_var *)cur->content;
		if (ft_strequ(var->key, key))
		{
			unset_var_with_var(var, lst, cur, prev);
			return (1);
		}
		prev = cur;
		cur = cur->next;
	}
	return (0);
}
