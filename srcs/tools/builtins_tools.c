/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 15:35:32 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/08 15:35:33 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "builtins.h"

int	is_builtin(t_alloc *alloc, const char *built_name)
{
	int	idx;

	idx = 0;
	while (alloc->builtins[idx].name)
	{
		if (ft_strequ(alloc->builtins[idx].name, built_name))
			return (1);
		idx += 1;
	}
	return (0);
}
