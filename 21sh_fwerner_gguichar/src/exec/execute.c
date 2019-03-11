/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/21 09:53:07 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 11:21:03 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "shell.h"
#include "cmd_inf.h"
#include "join_token_cmd.h"
#include "vars.h"
#include "builtins.h"
#include "execute.h"

void		child_exec_cmd_inf(t_shell *shell, t_cmd_inf *cmd_inf
		, const char *bin_path, char **arg_tab)
{
	char	**environ;

	environ = NULL;
	if (setup_redirections(cmd_inf))
	{
		environ = get_environ_from_list(shell->env);
		execve(bin_path, arg_tab, environ);
		ft_dprintf(2, "%s: %s: Cannot execute binary file\n"
				, ERR_PREFIX, bin_path);
	}
	free(environ);
	exit(126);
}

int			try_execute_builtin(t_shell *shell, t_cmd_inf *cmd_inf
		, char **arg_tab)
{
	size_t	idx;
	int		ret;

	idx = 0;
	while (shell->builtins[idx].name != NULL)
	{
		if (ft_strequ(shell->builtins[idx].name, cmd_inf->arg_lst->content))
		{
			if (!setup_redirections(cmd_inf))
				return (1);
			ret = shell->builtins[idx].builtin_fun(shell
					, ft_lstsize(cmd_inf->arg_lst), arg_tab);
			reset_redirections(cmd_inf);
			return (ret);
		}
		++idx;
	}
	return (-1);
}

static void	execute_cmd_inf(t_shell *shell, t_cmd_inf *cmd_inf)
{
	char	**arg_tab;
	int		ret;

	if (cmd_inf->pipe_cmd != NULL)
		execute_pipeline(shell, cmd_inf);
	else if (cmd_inf->arg_lst != NULL)
	{
		if ((arg_tab = arg_lst_to_tab(cmd_inf->arg_lst)) == NULL)
			return ;
		ret = try_execute_builtin(shell, cmd_inf, arg_tab);
		if (ret != -1)
			shell->last_status = ret;
		else
			execute_single_cmd(shell, cmd_inf);
		update_var(&(shell->env), "_"
				, arg_tab[ft_lstsize(cmd_inf->arg_lst) - 1]);
		free(arg_tab);
	}
}

void		execute_all(t_shell *shell, t_list *token_lst)
{
	t_list		*cmd_lst;
	t_list		*cur_cmd;

	cmd_lst = join_token_cmd(token_lst, shell);
	cur_cmd = cmd_lst;
	while (cur_cmd != NULL)
	{
		process_redir(shell, (t_cmd_inf *)cur_cmd->content);
		cur_cmd = cur_cmd->next;
	}
	cur_cmd = cmd_lst;
	while (cur_cmd != NULL)
	{
		execute_cmd_inf(shell, (t_cmd_inf *)cur_cmd->content);
		cur_cmd = cur_cmd->next;
	}
	ft_lstdel(&cmd_lst, del_cmd);
}
