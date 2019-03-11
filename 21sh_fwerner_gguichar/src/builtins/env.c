/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 14:53:18 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 14:57:29 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "shell.h"
#include "vars.h"
#include "builtins.h"

int	builtin_env(t_shell *shell, int argc, char **argv)
{
	t_list	*cur_env;
	t_var	*var;

	(void)argc;
	(void)argv;
	cur_env = shell->env;
	while (cur_env != NULL)
	{
		var = (t_var *)cur_env->content;
		ft_printf("%s=%s\n", var->key, var->value);
		cur_env = cur_env->next;
	}
	return (0);
}
