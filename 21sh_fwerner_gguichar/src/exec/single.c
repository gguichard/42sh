/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 18:18:26 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 11:25:31 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "cmd_inf.h"
#include "execute.h"

static void	single_fork(t_shell *shell, t_cmd_inf *cmd_inf
		, const char *bin_path, char **arg_tab)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
		ft_dprintf(2, "%s: %s: Unable to fork\n", ERR_PREFIX, bin_path);
	else if (pid == 0)
	{
		if (bin_path == NULL)
			exit(127);
		child_exec_cmd_inf(shell, cmd_inf, bin_path, arg_tab);
	}
	else
	{
		shell->fork_pids = ft_lstnew(&pid, sizeof(pid_t));
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status))
			print_sig_message(status);
		shell->last_status = WEXITSTATUS(status);
		ft_lstfree(&(shell->fork_pids));
	}
}

void		execute_single_cmd(t_shell *shell, t_cmd_inf *cmd_inf)
{
	t_error	error;
	char	*bin_path;
	char	**arg_tab;

	bin_path = get_cmd_inf_path(shell, cmd_inf, &error);
	arg_tab = NULL;
	if (error != ERRC_NOERROR)
	{
		ft_dprintf(2, "%s: %s: %s\n", ERR_PREFIX
				, cmd_inf->arg_lst->content, error_to_str(error));
		ft_strdel(&bin_path);
	}
	else if ((arg_tab = arg_lst_to_tab(cmd_inf->arg_lst)) == NULL)
	{
		ft_dprintf(2, "%s: %s: Unexpected error\n", ERR_PREFIX, bin_path);
		free(bin_path);
		return ;
	}
	single_fork(shell, cmd_inf, bin_path, arg_tab);
	free(arg_tab);
	free(bin_path);
}
