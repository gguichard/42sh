#include "shell.h"
#include "parser_lexer.h"
#include "operator.h"

static int		pipe_exec(t_ast *elem, t_var **lst_env, t_alloc **alloc, int *fd)
{
	int		ret;
	t_ast	*tmp;

	tmp = elem;
	while (!ft_strcmp("<<", tmp->input[0]))
		tmp = tmp->left;
	if (!tmp)
		exit(1);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	ret = analyzer(tmp, lst_env, alloc);
	exit(ret);
}

static void		write_pipe(t_ast *elem, int *fd)
{
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	write(STDOUT_FILENO, elem->heredoc, ft_strlen(elem->heredoc));
	exit(0);
}

int				complete_heredoc(t_ast *lst, t_alloc **alloc)
{
	while (lst)
	{
		if (lst->type == HEREDOC)
		{
			if (!heredoc_content(*alloc, lst, 0))
				return (0);
		}
		lst = lst->next;
	}
	return (1);
}

void			heredoc(t_ast *elem, t_var **lst_env, t_alloc **alloc)
{
	int		pid1;
	int		fd[2];
	int		pid2;

	if (!elem->heredoc || pipe(fd) == -1)
		return ;
	if (!(pid1 = fork()))
		write_pipe(elem, fd);
	else
	{
		g_pid = pid1;
		if (!(pid2 = fork()))
			pipe_exec(elem->left, lst_env, alloc, fd);
		else
		{
			g_pid = pid2;
			close(fd[1]);
			close(fd[0]);
			waitpid(pid1, NULL, 0);
			waitpid(pid2, &g_ret[0], 0);
			g_ret[1] = 1;
		}
	}
	ft_memdel((void **)&elem->heredoc);
}
