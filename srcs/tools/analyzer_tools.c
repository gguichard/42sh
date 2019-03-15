#include "shell.h"
#include "operator.h"
#include "builtins.h"
#include "parser_lexer.h"


int	dispatch_cmd(t_ast *elem, t_alloc *alloc, int no_fork)
{
	int					i;
	int					ret;

	i = 0;
	ret = 0	;
	while (alloc->builtins[i].name)
	{
		if (ft_strcmp(elem->input[0], alloc->builtins[i].name) == 0)
		{
			ret = alloc->builtins[i].built_fun(elem, alloc);
			break ;
		}
		i += 1;
	}
	if (!alloc->builtins[i].name)
		ret = exec_input(elem, alloc, no_fork);
	return (ret);
}

int	dispatch_logic(t_ast *elem, t_alloc *alloc, int no_fork)
{
	int		ret;

	ret = 1;
	if (ft_strcmp(elem->input[0], "&&") == 0)
	{
		ret = analyzer(elem, alloc, no_fork);
		if (ret == 0)
			ret = analyzer(elem, alloc, no_fork);
	}
	else if (ft_strcmp(elem->input[0], "||") == 0)
	{
		ret = analyzer(elem, alloc, no_fork);
		if (ret != 0)
			ret = analyzer(elem, alloc, no_fork);
	}
	return (ret);
}

int	dispatch_redir(t_ast *elem, t_alloc *alloc, int no_fork)
{
	static char		*tab_redir[7] = {">", ">>", ">>&", "<", "<<<", "<>", "<<"};
	int				i;

	i = 0;
	while (ft_strcmp(elem->input[0], tab_redir[i]) != 0
	&& ft_strcmp(elem->input[1], tab_redir[i]) != 0)
		i += 1;
	if (i == 0)
		i = redirection_1(elem, alloc, no_fork);
	else if (i == 1 || i == 2)
		i = redirection_2(elem, alloc, no_fork);
	else if (i == 3 || i == 4 || i == 5)
		i = redirection_3(elem, alloc, no_fork);
	else if (i == 6)
		i = heredoc(elem, alloc);
	else
		i = dispatch_agreg(elem, alloc, no_fork);
	return (i);
}

int	dispatch_operator(t_ast *elem, t_alloc *alloc, int no_fork)
{
	(void)no_fork;
	if (ft_strcmp(elem->input[0], "|") == 0)
		return (do_pipe(elem, alloc));
	else if (ft_strcmp(elem->input[0], "&") == 0)
		return (job_control(elem, alloc));
	return (1);
}

int	dispatch_agreg(t_ast *elem, t_alloc *alloc, int no_fork)
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
		ret = agreg_1(elem, alloc, no_fork);
	else if (i == 1)
		ret = agreg_2(elem, alloc, no_fork);
	else if (i == 2 && elem->input[1][0] != '-' && (!elem->input[2]
		|| elem->input[2][0] != '-'))
		ret = agreg_3(elem, alloc, no_fork);
	else if (i == 2)
		ret = agreg_4(elem, alloc, no_fork);
	else if (i == 3)
		ret = agreg_5(elem, alloc, no_fork);
	return (ret);
}
