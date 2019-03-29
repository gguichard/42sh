/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 20:36:38 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/29 13:07:29 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include "shell.h"
#include "execution.h"
#include "builtins.h"

int	try_builtin_execution(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	size_t	idx;
	int		ret;

	idx = 0;
	while (alloc->builtins[idx].name != NULL)
	{
		if (ft_strequ(elem->input[0], alloc->builtins[idx].name))
		{
			if (elem->left != NULL)
				analyzer(alloc, elem->left, opt);
			ret = alloc->builtins[idx].built_fun(elem, alloc);
			return (ret);
		}
		idx++;
	}
	return (-1);
}
