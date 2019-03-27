#include "shell.h"
#include "operator.h"
#include "parser_lexer.h"

int	redirection_1(t_ast *elem, t_alloc *alloc, t_exec_opt *opt)
{
	int	fd[10];
	int	i;

	i = 0;
	while (i < 10)
		fd[i++] = -1;
	(elem->right) ? elem->right->print = 1 : 0;
	while (ft_is_redir(elem, fd, alloc) == 1 && elem->left)
		elem = elem->left;
	if (elem->back && elem->type != REDIR)
		elem = elem->back;
	i = (elem->left) ? analyzer(elem->left, alloc, opt) : 1;
	reinit_fd(fd, alloc);
	return (i);
}

int	redirection_2(t_ast *elem, t_alloc *alloc, t_exec_opt *opt)
{
	int	fd[10];
	int	i;

	i = 0;
	while (i < 10)
		fd[i++] = -1;
	(elem->right) ? elem->right->print = 1 : 0;
	while (ft_is_redir(elem, fd, alloc) == 1 && elem->left)
		elem = elem->left;
	if (elem->back && elem->type != REDIR)
		elem = elem->back;
	i = (elem->left) ? analyzer(elem->left, alloc, opt) : 1;
	reinit_fd(fd, alloc);
	return (i);
}

int	redirection_3(t_ast *elem, t_alloc *alloc, t_exec_opt *opt)
{
	int	fd[10];
	int	i;

	i = 0;
	while (i < 10)
		fd[i++] = -1;
	while (ft_is_redir(elem, fd, alloc) == 1 && elem->left)
		elem = elem->left;
	if (elem->back && elem->type != REDIR)
		elem = elem->back;
	i = (elem->left) ? analyzer(elem->left, alloc, opt) : 1;
	reinit_fd(fd, alloc);
	return (i);
}
