/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 20:36:38 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/29 15:24:32 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "shell.h"
#include "execution.h"
#include "builtins.h"

int	try_builtin_execution(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	size_t		idx;
	int			ret;
	t_exec_opt	new_opt;

	idx = 0;
	while (alloc->builtins[idx].name != NULL)
	{
		if (ft_strequ(elem->input[0], alloc->builtins[idx].name))
		{
			ret = 0;
			new_opt = *opt;
			new_opt.should_save_fd = 1;
			if (elem->left != NULL)
				ret = analyzer(alloc, elem->left, &new_opt);
			if (ret == 0)
				ret = alloc->builtins[idx].built_fun(elem, alloc);
			use_rc_on_shell(&new_opt);
			return (ret);
		}
		idx++;
	}
	return (-1);
}
