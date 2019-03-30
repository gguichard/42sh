#include "libft.h"
#include "shell.h"
#include "cmdline.h"
#include "builtins.h"
#include "execution.h"
#include "exectable.h"
#include "hashtable.h"
#include "parser_lexer.h"
#include "str_cmd_inf.h"
#include "split_cmd_token.h"
#include "token_inf.h"
#include "job.h"
#include "error.h"

static void	lexer_parser(char *line, t_alloc *alloc)
{
	t_ast			*sort_ast;
	t_list			*lst_tk;
	t_exec_opt		exec_option;
	t_str_cmd_inf	scmd;

	sort_ast = NULL;
	lst_tk = NULL;
	ft_bzero(&exec_option, sizeof(t_exec_opt));
	if (!scmd_init(&scmd, line))
		return ;
	lst_tk = split_cmd_token(&scmd, alloc->aliastable);
	scmd_clean(&scmd);

	/*
	 **	VERIF TOKEN AND PRINT BEFORE PARSE
	 */
	// t_list	*tmp;
	// tmp = lst_tk;
	// while (tmp)
	// {
	// 	ft_printf("type: %d\ntoken: |%s|\n\n", get_tk(tmp)->type, get_tk(tmp)->token);
	// 	tmp = tmp->next;
	// }

	sort_ast = parser(lst_tk);
	ft_lstdel(&lst_tk, del_token);

	/*
	 ** COMPARAISON POUR RECONNAITRE LE JOB CONTROL
	 */
	// if (get_tk(lst_tk)->type == TK_CMD_SEP
	// && ft_strcmp(get_tk(lst_tk)->token, "&") == 0)
	// 	analyzer(sort_ast, alloc, TRUE);
	// else

	/*
	 ** PRINT AST AND REINIT NODE
	 */
	 if (sort_ast)
		read_sort_descent(sort_ast, 0);

	check_exit_cmd(sort_ast);
	alloc->ret_val = analyzer(alloc, sort_ast, &exec_option);

	//FUNCTION TO CLEAN / CLEAN TK_LIST MISSING
	del_ast(&sort_ast);
}

int		main(int argc, char **argv, char **environ)
{
	t_alloc	alloc;
	char	*input;

	g_jobs = 0;
	if (!setup_alloc(&alloc, argc, argv, environ))
		ft_dprintf(STDERR_FILENO, "Unable to setup environment\n");
	else
	{
		if (!init_cmdline(&alloc, &alloc.cmdline))
			ft_dprintf(STDERR_FILENO, "Unable to init term\n");
		else
		{
			load_history_file_entries(&alloc, &alloc.cmdline.history);
			while (1)
			{
				refresh_jobs();
				setup_term(&alloc.cmdline);
				input = read_cmdline(&alloc, &alloc.cmdline);
				reset_term(&alloc.cmdline);
				if (input != NULL)
				{
					lexer_parser(input, &alloc);
					free(input);
				}
			}
		}
	}
	terminate_all_jobs();
	del_alloc(&alloc);
	return (1);
}
