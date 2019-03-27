#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"

int	analyzer(t_ast *sort, t_alloc *alloc, t_exec_opt *opt)
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
		if (tmp->type != CMD_SEP || (tmp->type == CMD_SEP
				&& tmp->input[0][0] != ';'))
		{
			if (inhibitor(tmp, alloc) == 1)
			return (dispatch[tmp->type](tmp, alloc, opt));
		}
	}
	if (tmp && tmp->left && tmp->left->print == 0)
		analyzer(tmp->left, alloc, opt);
	if (tmp && tmp->right && tmp->right->print == 0)
		analyzer(tmp->right, alloc, opt);
	if (tmp && tmp->back)
		analyzer(tmp->back, alloc, opt);
	return (ret);
}
