/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/25 15:29:37 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/26 15:29:19 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "vars.h"

void	insert_new_elem(t_list **lst, t_list *new)
{
	t_list *tmp;

	tmp = *lst;
	if (!lst || !tmp)
		*lst = new;
	else if (ft_strcmp(((t_var *)new->content)->key,
			((t_var *)tmp->content)->key) <= 0)
	{
		new->next = tmp;
		*lst = new;
	}
	else
	{
		while (tmp && tmp->next && ft_strcmp(((t_var *)new->content)->key,
				((t_var *)tmp->next->content)->key) > 0)
			tmp = tmp->next;
		new->next = tmp->next;
		tmp->next = new;
	}
}

int	is_var_valid_identifier(const char *key)
{
	size_t	offset;

	if (ft_isdigit(key[0]))
		return (0);
	offset = 0;
	while (key[offset] != '\0')
	{
		if (!ft_isalnum(key[offset]) && key[offset] != '_')
			return (0);
		offset++;
	}
	return (1);
}
