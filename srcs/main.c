#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "cmdline.h"
#include "builtins.h"
#include "exectable.h"
#include "hashtable.h"
#include "parser_lexer.h"

static void	lexer_parser(char *line, t_alloc *alloc)
{
	char	***split_all_cmd;
	t_ast	*sort_ast;
	int		i;

	i = 0;
	sort_ast = NULL;
	split_all_cmd = lexer(line, alloc);
	while (split_all_cmd[i])
	{
		sort_ast = parser(split_all_cmd[i], alloc);
		analyzer(sort_ast, alloc, 0);
		// read_sort_descent(sort_ast, 1);
		// reinit_print(alloc->ast, 1);
		delete_str_tab(split_all_cmd[i]);
		del_lst_ast(&(alloc->ast));
		i += 1;
	}
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
