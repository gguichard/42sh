#include "shell.h"
#include "parser_lexer.h"
#include "operator.h"

static int		pipe_exec(t_ast *elem, t_alloc *alloc, int *fd)
{
	int		ret;
	t_ast	*tmp;
	int		no_fork;

	tmp = elem;
	while (!ft_strcmp("<<", tmp->input[0]))
		tmp = tmp->left;
	if (!tmp)
		exit(1);
	no_fork = do_not_fork(tmp, alloc);
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	ret = analyzer(tmp, alloc, no_fork);
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

int				complete_heredoc(t_ast *lst, t_alloc *alloc)
{
	while (lst)
	{
		if (lst->type == HEREDOC)
		{
			if (!heredoc_content(alloc, lst, 0))
				return (0);
		}
		lst = lst->next;
	}
	return (1);
}

int				heredoc(t_ast *elem, t_alloc *alloc)
{
	int		pid1;
	int		fd[2];
	int		pid2;

	if (!elem->heredoc || !elem->left || pipe(fd) == -1 || (pid1 = fork()) == -1)
		return (1);
	else if (!pid1)
		write_pipe(elem, fd);
	else
	{
		if ((pid2 = fork()) == -1)
			return (1);
		else if (!pid2)
			pipe_exec(elem->left, alloc, fd);
		else
		{
			close(fd[1]);
			close(fd[0]);
			waitpid(pid1, NULL, 0);
			waitpid(pid2, &alloc->ret_val, 0);
		}
	}
	ft_memdel((void **)&elem->heredoc);
	return (ret_status(alloc->ret_val, 0));
}
