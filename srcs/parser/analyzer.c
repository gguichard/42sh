#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"

static int	dispatch_cmd_sep(t_ast *sort, t_alloc *alloc, t_exec_opt *opt)
{
	if (sort->left)
		alloc->ret_val = analyzer(sort->left, alloc, opt);
	if (sort->right)
		alloc->ret_val = analyzer(sort->right, alloc, opt);
	return (0);
}

int	analyzer(t_ast *sort, t_alloc *alloc, t_exec_opt *opt)
{
	static t_dispatch	dispatch[] = { &dispatch_redir, &dispatch_cmd,
						&dispatch_assign, &dispatch_operator, &dispatch_logic };
	t_ast				*tmp;
	int					ret;

	ret = 0;
	tmp = sort;
	if (tmp && tmp->print == 0)
	{
		tmp->print = 1;
		if (tmp->type != CMD_SEP || (tmp->type == CMD_SEP
				&& tmp->input[0][0] != ';'))
		{
			if (inhibitor(tmp, alloc) == 1)
			return (dispatch[tmp->type](tmp, alloc, opt));
		}
		else
			return (dispatch_cmd_sep(sort, alloc, opt));
	}
	if (tmp && tmp->left && tmp->left->print == 0)
		return (analyzer(tmp->left, alloc, opt));
	else if (tmp && tmp->right && tmp->right->print == 0)
		return (analyzer(tmp->right, alloc, opt));
	else if (tmp && tmp->back)
		return (analyzer(tmp->back, alloc, opt));
	return (ret);
}
