/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/31 19:02:34 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/10 12:11:28 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include "libft.h"
#include "shell.h"
#include "error.h"
#include "execution.h"
#include "builtins.h"
#include "signals.h"

int	builtin_exec(t_ast *elem, t_alloc *alloc)
{
	t_error	err;
	int		hashable;
	char	*path_exec;

	if (elem->input[1] == NULL)
		return (0);
	hashable = 0;
	path_exec = exec_path(alloc, elem->input[1], &hashable, &err);
	if (path_exec == NULL)
	{
		ft_dprintf(STDERR_FILENO, "42sh: exec: %s: %s\n", elem->input[1]
				, error_to_str(err));
		return (127);
	}
	save_history_entries(alloc, &alloc->cmdline.history);
	sig_set_all(SIG_DFL);
	execute_cmd(alloc, elem->input + 1, path_exec);
	return (0);
}
