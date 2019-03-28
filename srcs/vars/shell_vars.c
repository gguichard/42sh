/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_vars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 17:51:53 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/27 16:25:58 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "vars.h"

char	*get_var_value(t_list *vars, const char *key)
{
	t_var	*var;

	var = get_var(vars, key);
	return (var == NULL ? "" : var->value);
}

char	*get_var_for_expand(t_alloc *alloc, const char *key)
{
	t_var	*var;
	int		tmp;

	var = NULL;
	if (ft_isalpha(key[0]) || key[0] == '_')
		var = get_var(alloc->vars, key);
	else
	{
		if (ft_strequ(key, "?"))
			return (ft_itoa(alloc->ret_val));
		// if (ft_strequ(key, "!") && alloc->last_bg != -1)
		// 	return (ft_itoa(alloc->last->bg));
		// if (ft_strequ(key, "$"))
		// 		return (ft_itoa(alloc->pid));
		if (ft_isdigit(key[0]))
		{
			tmp = ft_atoi(key);
			return (ft_strdup(alloc->argc <= tmp ? "" : alloc->argv[tmp]));
		}
	}
	return (ft_strdup(var == NULL ? "" : var->value));
}
