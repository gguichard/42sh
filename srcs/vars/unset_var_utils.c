/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_var_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 20:09:37 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/05 20:14:41 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "vars.h"

void	unset_var_with_var(t_var *var, t_list **lst, t_list *cur, t_list *prev)
{
	if (var->is_env != 2 && var->tmp_value != NULL)
	{
		free(var->value);
		var->value = var->tmp_value;
		var->tmp_value = NULL;
	}
	else
	{
		if (prev != NULL)
			prev->next = cur->next;
		else
			*lst = cur->next;
		ft_lstdelone(&cur, free_var);
	}
}
