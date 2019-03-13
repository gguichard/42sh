#include "shell.h"
#include "operator.h"
#include "parser_lexer.h"

void	redirection_1(t_ast *elem, t_var *lst_env, t_alloc *alloc)
{
	int	fd[10];
	int	i;

	i = 0;
	while (i < 10)
		fd[i++] = -1;
	(elem->right) ? elem->right->print = 1 : 0;
	while (ft_is_redir(elem, fd, alloc, lst_env) == 1 && elem->left)
		elem = elem->left;
	if (elem->back && elem->type != REDIR)
		elem = elem->back;
	if (elem->left)
		analyzer(elem->left, lst_env, alloc);
	reinit_fd(fd, alloc);
}

void	redirection_2(t_ast *elem, t_var *lst_env, t_alloc *alloc)
{
	int	fd[10];
	int	i;

	i = 0;
	while (i < 10)
		fd[i++] = -1;
	(elem->right) ? elem->right->print = 1 : 0;
	while (ft_is_redir(elem, fd, alloc, lst_env) == 1 && elem->left)
		elem = elem->left;
	if (elem->back && elem->type != REDIR)
		elem = elem->back;
	if (elem->left)
		analyzer(elem->left, lst_env, alloc);
	reinit_fd(fd, alloc);
}

void	redirection_3(t_ast *elem, t_var *lst_env, t_alloc *alloc)
{
	int	fd[10];
	int	i;

	i = 0;
	while (i < 10)
		fd[i++] = -1;
	while (ft_is_redir(elem, fd, alloc, lst_env) == 1 && elem->left)
		elem = elem->left;
	if (elem->back && elem->type != REDIR)
		elem = elem->back;
	if (elem->left)
		analyzer(elem->left, lst_env, alloc);
	reinit_fd(fd, alloc);
}
