/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 16:25:28 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/08 16:25:32 by tcollard         ###   ########.fr       */
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
