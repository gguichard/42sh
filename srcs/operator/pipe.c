#include "shell.h"
#include "operator.h"
#include "parser_lexer.h"

static int	process_pipe_left(t_ast *elem, t_var **lst_env, t_alloc *alloc, int *fd)
{
	int	ret;

	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	ret = analyzer(elem, lst_env, alloc);
	exit(ret);
}

static int	process_pipe_right(t_ast *elem, t_var **lst_env, t_alloc *alloc, int *fd)
{
	int	ret;

	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	ret = analyzer(elem, lst_env, alloc);
	exit(ret);
}

int			do_pipe(t_ast *elem, t_var **lst_env, t_alloc *alloc)
{
	int	pid1;
	int	pid2;
	int	fd[2];

	if (g_pid == -1 || pipe(fd) == -1 || !elem->right || !elem->left)
		return (0);
	if (!(pid1 = fork()))
		process_pipe_left(elem->left, lst_env, alloc, fd);
	else
	{
		g_pid = pid1;
		if (!(pid2 = fork()))
			process_pipe_right(elem->right, lst_env, alloc, fd);
		else
		{
			g_pid = pid2;
			close(fd[1]);
			close(fd[0]);
			waitpid(pid1, &(g_ret[0]), 0);
			waitpid(pid2, &(g_ret[0]), 0);
			g_ret[1] = 1;
		}
	}
	return (1);
}
