/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsetenv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 15:01:01 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 16:32:19 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "vars.h"
#include "hashtable.h"
#include "builtins.h"

int	builtin_unsetenv(t_shell *shell, int argc, char **argv)
{
	int	index;

	if (argc <= 1)
	{
		ft_dprintf(2, "%s: usage: unsetenv name...\n", ERR_PREFIX, argv[0]);
		return (1);
	}
	index = 1;
	while (index < argc)
	{
		if (ft_strequ("PATH", argv[index]))
			delete_hashentries(shell->exec_hashtable);
		unset_var(&(shell->env), argv[index]);
		index++;
	}
	return (0);
}
