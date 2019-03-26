/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vars.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 16:41:55 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/26 15:35:38 by tcollard         ###   ########.fr       */
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

int			create_var(t_list **lst, const char *key, const char *value)
{
	t_var	var;
	t_list	*elem;

	var.key = ft_strdup(key);
	var.value = ft_strdup(value);
	if (var.key != NULL && var.value != NULL)
	{
		elem = ft_lstnew(&var, sizeof(t_var));
		if (elem != NULL)
		{
			ft_lstpush(lst, elem);
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
		if ((dup = ft_strdup(value)) == NULL)
			return (0);
		free(var->value);
		var->value = dup;
		return (1);
	}
	return (create_var(lst, key, value));
}

int			unset_var(t_list **lst, const char *key)
{
	t_list	*prev;
	t_list	*curr;
	t_var	*var;

	prev = NULL;
	curr = *lst;
	while (curr != NULL)
	{
		var = (t_var *)curr->content;
		if (ft_strequ(var->key, key))
		{
			if (prev != NULL)
				prev->next = curr->next;
			ft_lstdelone(&curr, &free_var);
			return (1);
		}
		prev = curr;
		curr = curr->next;
	}
	return (0);
}
