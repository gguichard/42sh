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

// #include "get_next_line.h"
// #include "printf.h"

static void	lexer_parser(char *line, t_alloc *alloc)
{
	t_ast			*sort_ast;
	t_list			*lst_tk;
	t_str_cmd_inf	scmd;
	// int				j;

	sort_ast = NULL;
	lst_tk = NULL;
	split_cmd = NULL;
	scmd_init(&scmd, line);
	scmd_init(&scmd, split_cmd[i]);
	lst_tk = split_cmd_token(&scmd);
	if (!(sort_ast = parser(lst_tk, alloc)))
		break ;

		// j = 0;
		// while (lst_tk)
		// {
		// 	ft_printf("type[%d]: %d\nvalue[%d]: |%s|\n\n", j, get_tk(lst_tk)->type, j, get_tk(lst_tk)->token);
		// 	lst_tk = lst_tk->next;
		// 	j += 1;
		// }

		// analyzer(sort_ast, alloc, 0);
	read_sort_descent(sort_ast, 1);
		// reinit_print(alloc->ast, 1);
		// delete_str_tab(split_all_cmd[i]);
		// del_lst_ast(&(alloc->ast));

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
