#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "builtins.h"
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

	p_debug = 0;
	if (argc > 1 && !ft_strcmp(argv[1], "-d"))
		p_debug = 1;
	else if (argc > 1)
		ft_printf("usage : pour print l'ast -d\n");
	(void)argc;
	(void)argv;
	lst = 0;
	ft_bzero(&alloc, sizeof(t_alloc));
	env_cp(env, &lst);
	if ((alloc.exectable = make_exectable()) == NULL)
		return (1);
	set_alloc(&alloc, &lst);
	write(1, "> ", 2);
	while ((gnl_ret = get_next_line(STDIN_FILENO, &line)) > 0)
	{
		//parse line etc;
		lexer(line, &alloc);
		write(1, "> ", 2);
		ft_memdel((void **)&line);
	}
	ft_printf("GNL ret : %d\n", gnl_ret);
	return (0);
}
