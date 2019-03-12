/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyzer_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/16 19:17:43 by tcollard          #+#    #+#             */
/*   Updated: 2019/03/12 18:28:31 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	dispatch_cmd(t_ast *elem, t_env **lst_env, char **tab_path,
	t_alloc *alloc)
{
	int					i;
	int					ret;
	static char			*lst_built[6] = {"cd", "echo", "setenv", "unsetenv",
	"env", "exit"};
	static t_builtins	dispatch[] = { &cd_builtins, &echo_builtins,
		&setenv_builtins, &unsetenv_builtins, &env_builtins, &exit_builtins };

	i = 0;
	ret = 0	;
	while (i < 6)
	{
		if (ft_strcmp(elem->input[0], lst_built[i]) == 0)
			break ;
		i += 1;
	}
	if (i < 6)
	{
		ret = dispatch[i](elem, lst_env, alloc);
		delete_str_tab(tab_path);
	}
	else
		ret = exec_input(elem, *lst_env, tab_path);
	return (ret);
}

int	dispatch_logic(t_ast *elem, t_env **lst_env, char **tab_path,
	t_alloc *alloc)
{
	(void)tab_path;
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

int	dispatch_redir(t_ast *elem, t_env **lst_env, char **tab_path,
	t_alloc *alloc)
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
		dispatch_agreg(elem, lst_env, tab_path, alloc);
	return (1);
}

int	dispatch_operator(t_ast *elem, t_env **lst_env, char **tab_path,
	t_alloc *alloc)
{
	(void)tab_path;
	if (ft_strcmp(elem->input[0], "|") == 0)
		return (do_pipe(elem, lst_env, alloc));
	else if (ft_strcmp(elem->input[0], "&") == 0)
		return (job_control(elem, *lst_env));
	return (1);
}

int	dispatch_agreg(t_ast *elem, t_env **lst_env, char **tab_path,
	t_alloc *alloc)
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
		ret = agreg_1(elem, lst_env, tab_path, alloc);
	else if (i == 1)
		ret = agreg_2(elem, lst_env, tab_path, alloc);
	else if (i == 2 && elem->input[1][0] != '-' && (!elem->input[2]
		|| elem->input[2][0] != '-'))
		ret = agreg_3(elem, lst_env, tab_path, alloc);
	else if (i == 2)
		ret = agreg_4(elem, lst_env, tab_path, alloc);
	else if (i == 3)
		ret = agreg_5(elem, lst_env, tab_path, alloc);
	return (ret);
}
