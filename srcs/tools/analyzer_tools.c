#include "shell.h"
#include "operator.h"
#include "builtins.h"
#include "parser_lexer.h"


int	dispatch_cmd(t_ast *elem, t_var *lst_env, t_alloc *alloc)
{
	int					i;
	int					ret;

	i = 0;
	ret = 0	;
	while (alloc->builtins[i].name)
	{
		if (ft_strcmp(elem->input[0], alloc->builtins[i].name) == 0)
		{
			alloc->builtins[i].built_fun(elem, lst_env, alloc);
			break ;
		}
		i += 1;
	}
	if (!alloc->builtins[i].name)
		ret = exec_input(elem, lst_env, alloc);
	return (ret);
}

int	dispatch_logic(t_ast *elem, t_var *lst_env, t_alloc *alloc)
{
	if (ft_strcmp(elem->input[0], "&&") == 0)
	{
		analyzer(elem, lst_env, alloc);
		if (ret_status() == 0)
			analyzer(elem, lst_env, alloc);
		if (ret_status() == 0)
			return (0);
	}
	else if (ft_strcmp(elem->input[0], "||") == 0)
	{
		analyzer(elem, lst_env, alloc);
		if (ret_status() != 0)
			analyzer(elem, lst_env, alloc);
		if (ret_status() == 0)
			return (0);
	}
	return (-1);
}

int	dispatch_redir(t_ast *elem, t_var *lst_env, t_alloc *alloc)
{
	static char		*tab_redir[7] = {">", ">>", ">>&", "<", "<<<", "<>", "<<"};
	int				i;

	i = 0;
	while (ft_strcmp(elem->input[0], tab_redir[i]) != 0
	&& ft_strcmp(elem->input[1], tab_redir[i]) != 0)
		i += 1;
	if (i == 0)
		redirection_1(elem, lst_env, alloc);
	else if (i == 1 || i == 2)
		redirection_2(elem, lst_env, alloc);
	else if (i == 3 || i == 4 || i == 5)
		redirection_3(elem, lst_env, alloc);
	else if (i == 6)
		heredoc(elem, lst_env, alloc);
	else
		dispatch_agreg(elem, lst_env, alloc);
	return (1);
}

int	dispatch_operator(t_ast *elem, t_var *lst_env, t_alloc *alloc)
{
	if (ft_strcmp(elem->input[0], "|") == 0)
		return (do_pipe(elem, lst_env, alloc));
	else if (ft_strcmp(elem->input[0], "&") == 0)
		return (job_control(elem, lst_env));
	return (1);
}

int	dispatch_agreg(t_ast *elem, t_var *lst_env, t_alloc *alloc)
{
	static char	*tab_agreg[4] = {"&>", "&>>", ">&", "<&"};
	int			i;
	int			ret;

	i = 0;
	ret = 0;
	while (ft_strcmp(elem->input[0], tab_agreg[i]) != 0
	&& ft_strcmp(elem->input[1], tab_agreg[i]) != 0)
		i += 1;
	if (i == 0)
		ret = agreg_1(elem, lst_env, alloc);
	else if (i == 1)
		ret = agreg_2(elem, lst_env, alloc);
	else if (i == 2 && elem->input[1][0] != '-' && (!elem->input[2]
		|| elem->input[2][0] != '-'))
		ret = agreg_3(elem, lst_env, alloc);
	else if (i == 2)
		ret = agreg_4(elem, lst_env, alloc);
	else if (i == 3)
		ret = agreg_5(elem, lst_env, alloc);
	return (ret);
}
