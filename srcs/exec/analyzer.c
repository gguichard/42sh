#include <stdlib.h>
#include "shell.h"
#include "parser_lexer.h"
#include "execution.h"

static int	dispatch_logic(t_ast *elem, t_alloc *alloc, t_exec_opt *opt)
{
	int	ret;

	ret = 1;
	if (ft_strequ(elem->input[0], "&&"))
	{
		ret = analyzer(elem->left, alloc, opt);
		if (ret == 0)
			ret = analyzer(elem->right, alloc, opt);
	}
	else if (ft_strequ(elem->input[0], "||"))
	{
		ret = analyzer(elem->left, alloc, opt);
		if (ret != 0)
			ret = analyzer(elem->right, alloc, opt);
	}
	return (ret);
}

static int	dispatch_command(t_ast *elem, t_alloc *alloc, t_exec_opt *opt)
{
	int	idx;

	idx = 0;
	while (alloc->builtins[idx].name != NULL)
	{
		if (ft_strequ(elem->input[0], alloc->builtins[idx].name))
			return (alloc->builtins[idx].built_fun(elem, alloc));
		idx++;
	}
	return (exec_input(elem, alloc, opt));
}

int			analyzer(t_ast *elem, t_alloc *alloc, t_exec_opt *opt)
{
	if (elem != NULL)
	{
		if (elem->type == CMD_SEP)
		{
			analyzer(elem->left, alloc, opt);
			return (analyzer(elem->right, alloc, opt));
		}
		if (elem->type == LOGIC)
			return (dispatch_logic(elem, alloc, opt));
		if (elem->type == CMD)
			return (dispatch_command(elem, alloc, opt));
	}
	return (0);
}
