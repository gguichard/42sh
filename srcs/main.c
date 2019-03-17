#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "builtins.h"
#include "exectable.h"
#include "parser_lexer.h"

// #include "get_next_line.h"
// #include "printf.h"

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
		// lexer(line, &alloc);
		write(1, "> ", 2);
		ft_memdel((void **)&line);
	}
	del_alloc(&alloc);
	ft_printf("GNL ret : %d\n", gnl_ret);
	return (0);
}
