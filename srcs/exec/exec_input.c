/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_input.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 15:23:52 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/10 12:08:39 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "shell.h"
#include "vars.h"
#include "job.h"
#include "error.h"
#include "exectable.h"
#include "execution.h"
#include "signals.h"

void		execute_cmd(t_alloc *alloc, char **argv, char *path_exec)
{
	char	**tab_env;

	tab_env = get_environ_from_list(alloc->vars);
	if (tab_env == NULL)
		exit(127);
	execve(path_exec, argv, tab_env);
	ft_dprintf(STDERR_FILENO, "42sh: %s: not executable\n", argv[0]);
	ft_strdel(&path_exec);
	ft_strtab_free(tab_env);
	exit(126);
}

static void	prepare_execute(t_alloc *alloc, t_ast *elem
							, t_exec_opt *opt, char *path_exec)
{
	int	ret;

	ret = 0;
	update_var(&alloc->vars, "_", path_exec);
	sig_set_all(SIG_DFL);
	if (elem->left != NULL && (ret = analyzer(alloc, elem->left, opt)) != 0)
		exit(ret);
}

int			exec_input(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	t_error	err;
	int		hashable;
	char	*path_exec;
	pid_t	child;

	if ((path_exec = exec_path(alloc, elem->input[0], &hashable, &err)) == NULL)
	{
		exec_file_error(err, elem->input[0]);
		return (err == ERRC_FILENOTFOUND ? 127 : 126);
	}
	if (hashable == 1)
		set_exec_path(alloc->exectable, elem->input[0], path_exec, 1);
	if ((child = (!opt->fork) ? fork() : 0) == -1)
		return (1);
	else if (child == 0 && (opt->fork = 1) == 1)
	{
		prepare_execute(alloc, elem, opt, path_exec);
		execute_cmd(alloc, elem->input, path_exec);
	}
	ft_strdel(&path_exec);
	wait_pid(child, alloc, elem, opt);
	return (!opt->wait_hang ? ret_status(alloc->ret_val, child, 0, opt) : 0);
}
