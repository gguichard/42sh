/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   def_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 16:02:05 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 16:10:20 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "libft.h"
#include "shell.h"
#include "vars.h"

static void	increase_shlvl(t_shell *shell)
{
	t_var	*var;
	long	shlvl;
	char	*endptr;
	char	*tmp;

	var = get_var(shell->env, "SHLVL");
	if (var == NULL)
		shlvl = 0;
	else
	{
		shlvl = ft_strtol(var->value, &endptr, 10);
		if (*endptr != '\0' || shlvl < 0 || shlvl >= INT_MAX)
		{
			shlvl = 0;
			ft_dprintf(2, "%s: warning: Wrong shell level, resetting to 1\n"
					, ERR_PREFIX);
		}
	}
	tmp = ft_itoa((int)shlvl + 1);
	if (tmp != NULL)
	{
		update_var(&(shell->env), "SHLVL", tmp);
		free(tmp);
	}
}

void		setup_def_vars(t_shell *shell)
{
	t_var	*var;
	char	*cur_pwd;

	increase_shlvl(shell);
	var = get_var(shell->env, "PWD");
	if (var == NULL)
	{
		cur_pwd = getcwd(NULL, 0);
		if (cur_pwd != NULL)
		{
			create_var(&(shell->env), "PWD", cur_pwd);
			free(cur_pwd);
		}
	}
}
