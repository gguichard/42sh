#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "libft.h"
#include "shell.h"
#include "builtins.h"
#include "exectable.h"
#include "hashtable.h"
#include "parser_lexer.h"

static void	lexer_parser(char *line, t_alloc *alloc)
{
	char		***split_all_cmd;
	t_exec_opt	exec_option;
	t_ast		*sort_ast;
	int			i;

	i = 0;
	sort_ast = NULL;
	split_all_cmd = lexer(line, alloc);
	while (split_all_cmd[i])
	{
		sort_ast = parser(split_all_cmd[i], alloc);
		ft_bzero(&exec_option, sizeof(t_exec_opt));
		alloc->ret_val = analyzer(sort_ast, alloc, &exec_option);
		delete_str_tab(split_all_cmd[i]);
		del_lst_ast(&(alloc->ast));
		i += 1;
	}
}


//TODO faire un vrai main
void	signal_handle(int sig)
{
	t_job	*tmp;

	if (g_jobs)
	{
		tmp = g_jobs->content;
		if (sig == SIGTSTP)
			tmp->state = STOPPED;
		kill(tmp->pid, sig);
	}
}

int		main(int argc, char **argv, char **env)
{
	int		gnl_ret;
	char	*line;
	t_var	*lst;
	t_alloc	alloc;

	p_debug = 0;
	g_jobs = 0;
	signal(SIGTSTP, signal_handle);
	signal(SIGINT, signal_handle);
	if (argc > 1 && !ft_strcmp(argv[1], "-d"))
		p_debug = 1;
	else if (argc > 1)
		ft_printf("usage : pour print l'ast -d\n");
	lst = 0;
	ft_bzero(&alloc, sizeof(t_alloc));
	env_cp(env, &lst);
	set_alloc(&alloc, &lst);
	write(1, "> ", 2);
	while ((gnl_ret = get_next_line(STDIN_FILENO, &line)) > 0)
	{
		//parse line etc;
		lexer_parser(line, &alloc);
		refresh_jobs();
		write(1, "> ", 2);
		ft_memdel((void **)&line);
	}
	del_alloc(&alloc);
	ft_printf("GNL ret : %d\n", gnl_ret);
	return (0);
}
