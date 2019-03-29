#include <stdlib.h>
#include "shell.h"
#include "parser_lexer.h"
#include "redirect_inf.h"
#include "execution.h"

static int	dispatch_logic(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	int	ret;

	ret = 1;
	if (ft_strequ(elem->input[0], "&&"))
	{
		ret = analyzer(alloc, elem->left, opt);
		if (ret == 0)
			ret = analyzer(alloc, elem->right, opt);
	}
	else if (ft_strequ(elem->input[0], "||"))
	{
		ret = analyzer(alloc, elem->left, opt);
		if (ret != 0)
			ret = analyzer(alloc, elem->right, opt);
	}
	return (ret);
}

static int	dispatch_redirection(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	t_redirect_inf	redirect_inf;
	int				ret;

	if (!fill_redirect_inf(&redirect_inf, elem->input))
		return (1);
	process_redir(&redirect_inf);
	ret = 0;
	if (!setup_redirection(&redirect_inf))
		ret = 1;
	clean_redirect(&redirect_inf);
	if (ret == 0)
		ret = analyzer(alloc, elem->left, opt);
	return (ret);
}

static int	dispatch_command(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	int	ret;

	ret = try_builtin_execution(alloc, elem, opt);
	if (ret == -1)
		ret = exec_input(alloc, elem, opt);
	return (ret);
}

int			analyzer(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	if (elem != NULL)
	{
		if (elem->type == AST_CMD_SEP)
		{
			analyzer(alloc, elem->left, opt);
			return (analyzer(alloc, elem->right, opt));
		}
		if (elem->type == AST_LOGIC)
			return (dispatch_logic(alloc, elem, opt));
		if (elem->type == AST_CMD)
			return (dispatch_command(alloc, elem, opt));
		if (elem->type == AST_REDIR)
			return (dispatch_redirection(alloc, elem, opt));
		if (elem->type == AST_PIPE)
			return (do_pipe(alloc, elem, opt));
		if (elem->type == AST_JOB)
			return (job_control(alloc, elem, opt));
	}
	return (0);
}
