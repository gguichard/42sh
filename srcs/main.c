#include <unistd.h>
#include <stdlib.h>
#include "../includes/shell.h"
#include "../includes/parser_lexer.h"

// #include "get_next_line.h"
// #include "printf.h"

//TODO faire un vrai main
int		main(int argc, char **argv)
{
	int		gnl_ret;
	char	*line;
	t_var	*lst_env;
	t_alloc	alloc;

	(void)argc;
	(void)argv;
	lst_env = NULL;
	while ((gnl_ret = get_next_line(STDOUT_FILENO, &line)) > 0)
	{
		//parse line etc;
		write(1, "> ", 2);
		lexer(line, &lst_env, &alloc);
		// free(line);
	}
	ft_printf("GNL ret : %d\n", gnl_ret);
	return (0);
}
