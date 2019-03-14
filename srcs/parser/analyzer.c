#include "shell.h"
#include "parser_lexer.h"

int	analyzer(t_ast *sort, t_alloc *alloc)
{
	static t_dispatch	dispatch[] = { &dispatch_cmd, &dispatch_redir,
						&dispatch_redir, &dispatch_agreg, &dispatch_operator,
						&dispatch_logic };
	t_ast				*tmp;
	int					ret;

	ret = 0;
	tmp = sort;
	if (tmp && tmp->print == 0)
	{
		tmp->print = 1;
		return (dispatch[tmp->type](tmp, alloc));
	}
	if (tmp && tmp->left && tmp->left->print == 0)
		return (analyzer(tmp->left, alloc));
	else if (tmp && tmp->right && tmp->right->print == 0)
		return (analyzer(tmp->right, alloc));
	else if (tmp && tmp->back)
		return (analyzer(tmp->back, alloc));
	return (ret);
}
