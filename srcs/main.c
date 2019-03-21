#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "cmdline.h"
#include "builtins.h"
#include "exectable.h"
#include "hashtable.h"
#include "parser_lexer.h"
#include "str_cmd_inf.h"
#include "split_cmd_token.h"
#include "token_inf.h"

static void	lexer_parser(char *line, t_alloc *alloc)
{
	t_ast			*sort_ast;
	t_list			*lst_tk;
	t_str_cmd_inf	scmd;


	sort_ast = NULL;
	lst_tk = NULL;
	scmd_init(&scmd, line);
	lst_tk = split_cmd_token(&scmd);

/*
**	VERIF TOKEN AND PRINT BEFORE PARSE
*/
	(void)alloc;
	t_list	*tmp;
	tmp = lst_tk;
	while (tmp)
	{
		ft_printf("type: %d\ntoken: |%s|\n\n", get_tk(tmp)->type, get_tk(tmp)->token);
		tmp = tmp->next;
	}

	while (lst_tk)
	{
		if (!(sort_ast = parser(&lst_tk, alloc)))
		{
			ft_printf("ERROR BREAK\n");
			break ;
		}
/*
** COMPARAISON POUR RECONNAITRE LE JOB CONTROL
*/
	// if (get_tk(lst_tk)->type == TK_CMD_SEP
	// && ft_strcmp(get_tk(lst_tk)->token, "&") == 0)
	// 	analyzer(sort_ast, alloc, TRUE);
	// else
	// 	analyzer(sort_ast, alloc, FALSE);

/*
** PRINT AST AND REINIT NODE
*/
		if (sort_ast)
		{
			read_sort_descent(sort_ast, 1);
			reinit_print(alloc->ast, 1);
		}
		if (lst_tk)
			lst_tk = lst_tk->next;
//FUNCTION TO CLEAN AST
	}
// FUNCTION TO CLEAN LST_TK
}

//TODO faire un vrai main
int		main(int argc, char **argv, char **env)
{
	t_var	*lst;
	t_alloc	alloc;
	char	*input;

	(void)argc;
	(void)argv;
	lst = 0;
	ft_bzero(&alloc, sizeof(t_alloc));
	env_cp(env, &lst);
	set_alloc(&alloc, &lst);
	init_cmdline(&alloc.cmdline);
	while (1)
	{
		setup_term(&alloc.cmdline);
		input = read_cmdline(&alloc.cmdline);
		reset_term(&alloc.cmdline);
		if (input != NULL)
		{
			lexer_parser(input, &alloc);
			free(input);
		}
	}
	del_alloc(&alloc);
	return (0);
}
