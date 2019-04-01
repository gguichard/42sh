/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 20:36:38 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/31 19:42:31 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "shell.h"
#include "execution.h"
#include "builtins.h"

static int	execute_builtin(t_alloc *alloc, t_ast *elem, t_exec_opt *opt
		, const t_builtin *builtin)
{
	int			ret;
	t_exec_opt	new_opt;

	ret = 0;
	new_opt = *opt;
	if (builtin->built_fun == builtin_exec)
		new_opt.fork = 1;
	new_opt.from_builtin = 1;
	if (elem->left != NULL)
		ret = analyzer(alloc, elem->left, &new_opt);
	if (ret == 0)
		ret = builtin->built_fun(elem, alloc);
	use_rc_on_shell(&new_opt);
	return (ret);
}

int			try_builtin_execution(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	size_t	idx;

	idx = 0;
	while (alloc->builtins[idx].name != NULL)
	{
		if (ft_strequ(elem->input[0], alloc->builtins[idx].name))
			return (execute_builtin(alloc, elem, opt, alloc->builtins + idx));
		idx++;
	}
	return (-1);
}
