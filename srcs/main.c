#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "shell.h"
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
		alloc->ast = sort_ast;
/*
** COMPARAISON POUR RECONNAITRE LE JOB CONTROL
*/
	// if (get_tk(lst_tk)->type == TK_CMD_SEP
	// && ft_strcmp(get_tk(lst_tk)->token, "&") == 0)
	// 	analyzer(sort_ast, alloc, TRUE);
	// else
	// 	analyzer(sort_ast, alloc, FALSE);

		analyzer(sort_ast, alloc, 0);


/*
** PRINT AST AND REINIT NODE
*/
		ft_printf("\n\nPRINT AST:\n\n");
		if (sort_ast)
		{
			read_sort_descent(sort_ast, 1);
			reinit_print(alloc->ast, 1);
		}
		if (lst_tk)
			lst_tk = lst_tk->next;
//FUNCTION TO CLEAN AST
		del_ast(&sort_ast);
	}
// FUNCTION TO CLEAN LST_TK
}


//TODO faire un vrai main
int		main(int argc, char **argv, char **env)
{
	int		gnl_ret;
	char	*line;
	t_var	*lst;
	t_alloc	alloc;

	(void)argc;
	(void)argv;
	lst = 0;
	ft_bzero(&alloc, sizeof(t_alloc));
	env_cp(env, &lst);
	set_alloc(&alloc, &lst);
	write(1, "> ", 2);
	while ((gnl_ret = get_next_line(STDIN_FILENO, &line)) > 0)
	{
		//parse line etc;
		lexer_parser(line, &alloc);
		write(1, "> ", 2);
		ft_memdel((void **)&line);
	}
	del_alloc(&alloc);
	ft_printf("GNL ret : %d\n", gnl_ret);
	return (0);
}
