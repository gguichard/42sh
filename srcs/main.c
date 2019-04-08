/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 13:32:08 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/08 16:35:50 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "cmdline.h"
#include "execution.h"
#include "parser_lexer.h"
#include "str_cmd_inf.h"
#include "split_cmd_token.h"
#include "builtins.h"
#include "job.h"
#include "signals.h"

int			lexer_parser(const char *line, t_alloc *alloc, int fork)
{
	t_str_cmd_inf	scmd;
	t_list			*lst_tk;
	t_exec_opt		exec_opt;
	int				ret;

	if (!scmd_init(&scmd, line))
		return (1);
	lst_tk = split_cmd_token(&scmd, alloc->aliastable);
	scmd_clean(&scmd);
	if (!(alloc->ast = parser(lst_tk)))
	{
		ft_lstdel(&lst_tk, del_token);
		return (1);
	}
	ft_lstdel(&lst_tk, del_token);
	sigs_wait_line(alloc);
	ft_memset(&exec_opt, 0, sizeof(t_exec_opt));
	exec_opt.fork = fork;
	ret = analyzer(alloc, alloc->ast, &exec_opt);
	if (g_sig == SIGINT)
		g_sig = 0;
	del_ast(&alloc->ast);
	return (ret);
}

static void	shell_loop(t_alloc *alloc)
{
	while (1)
	{
		refresh_jobs();
		setup_term(&alloc->cmdline);
		read_cmdline(alloc, &alloc->cmdline);
		reset_term(&alloc->cmdline);
		if (alloc->full_input != NULL)
		{
			set_sigmask(SIG_BLOCK);
			alloc->ret_val = lexer_parser(alloc->full_input, alloc, 0);
			ft_strdel(&alloc->full_input);
			set_signals_handlers();
		}
	}
}

int			main(int argc, char **argv, char **environ)
{
	t_alloc	alloc;

	g_sig = 0;
	g_jobs = NULL;
	if (!setup_alloc(&alloc, argc, argv, environ))
		ft_dprintf(STDERR_FILENO, "42sh: unable to setup environment\n");
	else if (!init_cmdline(&alloc, &alloc.cmdline))
		ft_dprintf(STDERR_FILENO, "42sh: unable to init term\n");
	else
	{
		source_rc_file(&alloc);
		load_history_file_entries(&alloc, &alloc.cmdline.history);
		set_signals_handlers();
		shell_loop(&alloc);
	}
	terminate_all_jobs(SIGTERM);
	del_alloc(&alloc);
	sig_reset();
	return (1);
}
