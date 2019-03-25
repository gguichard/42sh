#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"

int	analyzer(t_ast *sort, t_alloc *alloc, int no_fork)
{
	static t_dispatch	dispatch[] = { &dispatch_cmd, &dispatch_redir,
						&dispatch_redir, &dispatch_assign, &dispatch_operator,
						&dispatch_logic };
	t_ast				*tmp;
	int					ret;

	ret = 0;
	tmp = sort;
	if (tmp && tmp->print == 0)
	{
		tmp->print = 1;
		if (inhibitor(tmp, alloc) == 1)
		// if (expand(tmp, alloc) == 1)
			return (dispatch[tmp->type](tmp, alloc, no_fork));
	}
	if (tmp && tmp->left && tmp->left->print == 0)
		return (analyzer(tmp->left, alloc, no_fork));
	else if (tmp && tmp->right && tmp->right->print == 0)
		return (analyzer(tmp->right, alloc, no_fork));
	else if (tmp && tmp->back)
		return (analyzer(tmp->back, alloc, no_fork));
	return (ret);
}
