/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_vars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 20:20:24 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/09 14:46:02 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "shell.h"
#include "vars.h"

const char	*get_var_value(t_list *vars, const char *key)
{
	t_var	*var;

	var = get_var(vars, key);
	return ((var == NULL || var->value == NULL) ? "" : var->value);
}

const char	*get_var_value_or_null(t_list *vars, const char *key)
{
	t_var	*var;

	var = get_var(vars, key);
	return (var == NULL ? NULL : var->value);
}

static char	*get_var_special_cases(t_alloc *alloc, const char *key)
{
	int	tmp;

	if (ft_strequ(key, "?"))
		return (ft_itoa(alloc->ret_val));
	if (ft_strequ(key, "!") && alloc->last_bg)
		return (ft_itoa(alloc->last_bg));
	if (ft_strequ(key, "$"))
		return (ft_itoa(alloc->ppid));
	if (ft_isdigit(key[0]))
	{
		tmp = ft_atoi(key);
		return (ft_strdup(alloc->argc <= tmp ? "" : alloc->argv[tmp]));
	}
	return (ft_strdup(""));
}

char		*get_var_for_expand(t_alloc *alloc, const char *key)
{
	t_var	*var;

	if (key == NULL)
		return (NULL);
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (get_var_special_cases(alloc, key));
	var = get_var(alloc->vars, key);
	if (var == NULL || var->is_env == 2)
		return (ft_strdup(""));
	else
		return (ft_strdup(var->tmp_value != NULL
					? var->tmp_value : var->value));
}
