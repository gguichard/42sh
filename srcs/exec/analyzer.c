#include <stdlib.h>
#include "shell.h"
#include "parser_lexer.h"
#include "operator.h"
#include "redirect_inf.h"
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
	int	ret;

	ret = try_builtin_execution(alloc, elem, opt);
	if (ret == -1)
		ret = exec_input(elem, alloc, opt);
	return (ret);
}

int			analyzer(t_ast *elem, t_alloc *alloc, t_exec_opt *opt)
{
	t_redirect_inf	redirect_inf;

	if (elem != NULL)
	{
		if (elem->type == AST_CMD_SEP)
		{
			analyzer(elem->left, alloc, opt);
			return (analyzer(elem->right, alloc, opt));
		}
		if (elem->type == AST_LOGIC)
			return (dispatch_logic(elem, alloc, opt));
		if (elem->type == AST_CMD)
			return (dispatch_command(elem, alloc, opt));
		if (elem->type == AST_REDIR)
		{
			if (!fill_redirect_inf(&redirect_inf, elem->input))
				return (1);
			process_redir(&redirect_inf);
			if (!setup_redirection(&redirect_inf))
				return (1);
			return (analyzer(elem->left, alloc, opt));
		}
		if (elem->type == AST_PIPE)
			return (do_pipe(elem, alloc, opt));
		if (elem->type == AST_JOB)
			return (job_control(elem, alloc, opt));
	}
	return (0);
}
