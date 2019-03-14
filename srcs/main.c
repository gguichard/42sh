#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "exectable.h"
#include "parser_lexer.h"

// #include "get_next_line.h"
// #include "printf.h"

//TODO faire un vrai main
int		main(int argc, char **argv, char **env)
{
	int		gnl_ret;
	char	*line;
	t_var	*lst;
	t_alloc	alloc;

	(void)argc;
	(void)argv;
	lst = NULL;

	int i;
	i = 0;
	env_cp(env, &lst);
	ft_bzero(&alloc, sizeof(t_alloc));
	if ((alloc.exectable = make_exectable()) == NULL)
		return (1);
	alloc = set_alloc(lst);
	write(1, "> ", 2);
	while ((gnl_ret = get_next_line(STDIN_FILENO, &line)) > 0)
	{
		//parse line etc;
		lexer(line, lst, &alloc);
		write(1, "> ", 2);
		//free(line);
	}
	ft_printf("GNL ret : %d\n", gnl_ret);
	return (0);
}
