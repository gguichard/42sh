/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/28 20:36:38 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/10 12:59:39 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include "shell.h"
#include "signals.h"
#include "execution.h"
#include "builtins.h"

static void	handler_redir(int sig)
{
	g_sig = sig;
	write(1, "\n", 1);
}

void		sig_redir(void)
{
	struct sigaction	act;
	sigset_t			mask;

	sigfillset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_handler = handler_redir;
	sigaction(SIGINT, &act, 0);
	sigemptyset(&mask);
	sigaddset(&mask, SIGINT);
}

static int	execute_builtin(t_alloc *alloc, t_ast *elem, t_exec_opt *opt
		, const t_builtin *builtin)
{
	int					ret;
	t_exec_opt			new_opt;

	ret = 0;
	new_opt = *opt;
	if (builtin->built_fun == builtin_exec)
		new_opt.fork = 1;
	new_opt.from_cmd = 1;
	sig_redir();
	if (elem->left != NULL)
		ret = analyzer(alloc, elem->left, &new_opt);
	set_sigread(1, 0, 0);
	if (ret == 0 && g_sig != SIGINT)
		ret = builtin->built_fun(elem, alloc);
	use_rc_on_shell(&new_opt);
	return ((g_sig == SIGINT) ? 1 : ret);
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
