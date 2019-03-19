#include "shell.h"
#include "operator.h"
#include "parser_lexer.h"

int			do_not_fork(t_ast *elem, t_alloc *alloc)
{
	int		x;

	while (elem && elem->type != CMD)
	{
		if (elem->type == OPERATOR && !ft_strcmp(elem->input[0], "|"))
			break ;
		elem = elem->left;
	}
	if (elem && elem->type == CMD)
	{
		x = 0;
		while (alloc->builtins[x].name && ft_strcmp(elem->input[0], alloc->builtins[x].name))
			x += 1;
	}
	if (!elem || elem->type == OPERATOR || alloc->builtins[x].name)
		return (0);
	return (1);
}

static int	process_pipe_left(t_ast *elem, t_alloc *alloc, int *fd)
{
	int	ret;
	int	no_fork;

	no_fork = do_not_fork(elem, alloc);
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	ret = analyzer(elem, alloc, no_fork);
	exit(ret);
}

static int	process_pipe_right(t_ast *elem, t_alloc *alloc, int *fd)
{
	int	no_fork;
	int	ret;

	no_fork = do_not_fork(elem, alloc);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	ret = analyzer(elem, alloc, no_fork);
	exit(ret);
}

int			do_pipe(t_ast *elem, t_alloc *alloc)
{
	int		pid1;
	int		pid2;
	int		fd[2];

	if (!elem->right || !elem->left || pipe(fd) == -1 || (pid1 = fork()) == -1)
		return (1);
	else if (!pid1)
		process_pipe_left(elem->left, alloc, fd);
	else
	{
		if ((pid2 = fork()) == -1)
			return (1);
		else if (!pid2)
			process_pipe_right(elem->right, alloc, fd);
		else
		{
			close(fd[1]);
			close(fd[0]);
			waitpid(pid1, &alloc->ret_val, 0);
			waitpid(pid2, &alloc->ret_val, 0);
		}
	}
	return (ret_status(alloc->ret_val));
}
