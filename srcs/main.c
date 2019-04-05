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

void	lexer_parser(const char *line, t_alloc *alloc)
{
	t_str_cmd_inf	scmd;
	t_list			*lst_tk;
	t_ast			*sort_ast;
	t_exec_opt		exec_opt;

	if (!scmd_init(&scmd, line))
		return ;
	sigs_wait_line(alloc);
	lst_tk = split_cmd_token(&scmd, alloc->aliastable);
	scmd_clean(&scmd);
	sigs_wait_line(alloc);
	if (!(sort_ast = parser(lst_tk)))
		return ;
	ft_lstdel(&lst_tk, del_token);
	sigs_wait_line(alloc);
	// if (sort_ast)
	// 	read_sort_descent(sort_ast, 0);
	ft_memset(&exec_opt, 0, sizeof(t_exec_opt));
	alloc->ret_val = analyzer(alloc, sort_ast, &exec_opt);
	if (g_sig == SIGINT)
		g_sig = 0;
	del_ast(&sort_ast);
}

int		main(int argc, char **argv, char **environ)
{
	t_alloc	alloc;

	g_sig = 0;
	g_jobs = NULL;
	signal(SIGTTOU, SIG_IGN);
	set_signals_handlers();
	if (!setup_alloc(&alloc, argc, argv, environ))
		ft_dprintf(STDERR_FILENO, "42sh: unable to setup environment\n");
	else if (!init_cmdline(&alloc, &alloc.cmdline))
		ft_dprintf(STDERR_FILENO, "42sh: unable to init term\n");
	else
	{
		source_rc_file(&alloc);
		load_history_file_entries(&alloc, &alloc.cmdline.history);
		while (1)
		{
			refresh_jobs();
			setup_term(&alloc.cmdline);
			read_cmdline(&alloc, &alloc.cmdline);
			reset_term(&alloc.cmdline);
			if (alloc.full_input != NULL)
			{
				// set_sigmask(SIG_BLOCK);
				lexer_parser(alloc.full_input, &alloc);
				ft_strdel(&alloc.full_input);
				set_signals_handlers();
			}
		}
	}
	terminate_all_jobs(SIGTERM);
	del_alloc(&alloc);
	sig_reset();
	return (1);
}
