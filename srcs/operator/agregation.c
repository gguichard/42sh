#include "shell.h"
#include "parser_lexer.h"
#include "operator.h"

int	agreg_1(t_ast *elem, t_alloc *alloc, int no_fork)
{
	int	fd[10];
	int	i;

	i = 0;
	while (i < 10)
		fd[i++] = -1;
	(elem->right) ? elem->right->print = 1 : 0;
	while ((ft_is_agreg(elem, fd, alloc) == 1
		|| ft_is_redir(elem, fd, alloc) == 1) && elem->left)
		elem = elem->left;
	(elem->type != AGREG) ? elem = elem->back : 0;
	dup2(fd[1], 1);
	dup2(fd[2], 2);
	(elem) ? analyzer(elem->left, alloc, no_fork) : 0;
	reinit_fd(fd, alloc);
	return (0);
}

int	agreg_2(t_ast *elem, t_alloc *alloc, int no_fork)
{
	int	fd[10];
	int	i;

	i = 0;
	while (i < 10)
		fd[i++] = -1;
	(elem->right) ? elem->right->print = 1 : 0;
	while ((ft_is_agreg(elem, fd, alloc) == 1
		|| ft_is_redir(elem, fd, alloc) == 1) && elem->left)
		elem = elem->left;
	(elem->type != AGREG) ? elem = elem->back : 0;
	dup2(fd[1], 1);
	dup2(fd[2], 2);
	(elem) ? analyzer(elem->left, alloc, no_fork) : 0;
	reinit_fd(fd, alloc);
	return (0);
}

int	agreg_3(t_ast *elem, t_alloc *alloc, int no_fork)
{
	int	fd_redir;
	int	ret1;
	int	dig;
	int	fd[10];
	int	i;

	i = 0;
	while (i < 10)
		fd[i++] = -1;
	fd_redir = -1;
	ret1 = 0;
	dig = ft_isdigit(elem->input[0][0]);
	fd_redir = (dig == 1) ? ft_atoi(elem->input[0]) : 1;
	(elem->right) ? elem->right->print = 1 : 0;
	while (((ret1 = ft_is_agreg(elem, fd, alloc)) == 1
		|| ft_is_redir(elem, fd, alloc) == 1) && elem->left)
		elem = elem->left;
	(elem->type != AGREG) ? elem = elem->back : 0;
	if (ret1 == -1)
		return (-1);
	(elem) ? analyzer(elem->left, alloc, no_fork) : 0;
	reinit_fd(fd, alloc);
	return (0);
}

int	agreg_4(t_ast *elem, t_alloc *alloc, int no_fork)
{
	int	fd_close;
	int	fd_save;
	int	tmp;

	fd_close = (ft_isdigit(elem->input[0][0]) == 1) ?
		ft_atoi(elem->input[0]) : 1;
	fd_save = dup(fd_close);
	while (fd_save < 3)
	{
		tmp = fd_save;
		fd_save = dup(fd_close);
		close(tmp);
	}
	alloc->fd[fd_close] = fd_save;
	close(fd_close);
	analyzer(elem->left, alloc, no_fork);
	dup2(fd_save, fd_close);
	alloc->fd[fd_close] = 0;
	return (0);
}

int	agreg_5(t_ast *elem, t_alloc *alloc, int no_fork)
{
	int	fd_close;
	int	fd_save;

	fd_close = (ft_isdigit(elem->input[0][0]) == 1) ?
		ft_atoi(elem->input[0]) : 1;
	fd_save = dup(fd_close);
	close(fd_close);
	analyzer(elem->left, alloc, no_fork);
	dup2(fd_save, fd_close);
	return (0);
}
