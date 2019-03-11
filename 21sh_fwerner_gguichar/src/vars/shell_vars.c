/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_vars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 17:51:53 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/28 14:19:29 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vars.h"

char	*get_shell_var(t_shell *shell, const char *key)
{
	t_var	*var;
	int		tmp;

	var = NULL;
	if (ft_isalpha(key[0]) || key[0] == '_')
	{
		var = get_var(shell->env, key);
		if (var == NULL)
			var = get_var(shell->local, key);
	}
	else
	{
		if (ft_strequ(key, "?"))
			return (ft_itoa(shell->last_status));
		if (ft_isdigit(key[0]))
		{
			tmp = ft_atoi(key);
			return (ft_strdup(shell->argc <= tmp ? "" : shell->argv[tmp]));
		}
	}
	return (ft_strdup(var == NULL ? "" : var->value));
}
