#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "libft.h"
#include "shell.h"
#include "builtins.h"
#include "exectable.h"
#include "parser_lexer.h"

// #include "get_next_line.h"
// #include "printf.h"

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

void	check_jobs_finish(void)
{
	t_list	*prev;
	t_list	*tmp;
	t_list	*aft;
	t_job	*job;

	tmp = g_jobs;
	prev = 0;
	while (tmp)
	{
		job = tmp->content;
		aft = tmp->next;
		if (job->state == DONE)
		{
			if (prev)
				prev->next = aft;
			else
				g_jobs = aft;
			ft_memdel((void **)&(job->cmd));
			ft_memdel((void **)&(tmp->content));
			ft_memdel((void **)&tmp);
			tmp = aft;
		}
		else
		{
			prev = tmp;
			tmp = tmp->next;
		}
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
		check_jobs_finish();
		if (g_jobs)
			write(1, "\n", 1);
		write(1, "> ", 2);
		ft_memdel((void **)&line);
	}
	ft_printf("GNL ret : %d\n", gnl_ret);
	return (0);
}
