/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/22 14:34:46 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 20:53:25 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "execute.h"

static t_list	*create_pipeline(t_cmd_inf *cmd_inf)
{
	t_cmd_inf	*cur_cmd_inf;
	t_list		*pipeline;
	t_list		*elem;
	t_pipe		data;

	cur_cmd_inf = cmd_inf;
	pipeline = NULL;
	while (cur_cmd_inf != NULL)
	{
		data.is_leftmost = cmd_inf == cur_cmd_inf;
		data.is_rightmost = cur_cmd_inf->pipe_cmd == NULL;
		data.in_fd = -1;
		data.out_fd = -1;
		data.cmd_inf = cur_cmd_inf;
		elem = ft_lstnew(&data, sizeof(t_pipe));
		if (elem == NULL)
			return (ft_lstfree(&pipeline));
		ft_lstpush(&pipeline, elem);
		cur_cmd_inf = cur_cmd_inf->pipe_cmd;
	}
	return (pipeline);
}

static int		exec_pipe(t_shell *shell, t_pipe *pipe)
{
	char		**arg_tab;
	int			ret;
	t_error		error;
	char		*bin_path;

	if ((arg_tab = arg_lst_to_tab(pipe->cmd_inf->arg_lst)) == NULL)
		return (0);
	ret = try_execute_builtin(shell, pipe->cmd_inf, arg_tab);
	if (ret != -1)
	{
		free(arg_tab);
		exit(ret);
	}
	bin_path = get_cmd_inf_path(shell, pipe->cmd_inf, &error);
	if (error == ERRC_NOERROR)
		child_exec_cmd_inf(shell, pipe->cmd_inf, bin_path, arg_tab);
	else
		ft_dprintf(2, "%s: %s: %s\n", ERR_PREFIX
				, pipe->cmd_inf->arg_lst->content, error_to_str(error));
	free(bin_path);
	free(arg_tab);
	return (1);
}

static pid_t	pipe_fork(t_shell *shell, t_list *curr)
{
	t_pipe	*pipe;
	pid_t	pid;

	pipe = (t_pipe *)curr->content;
	pid = fork();
	if (pid > 0)
		ft_lstpush(&(shell->fork_pids), ft_lstnew(&(pid), sizeof(pid_t)));
	else if (pid < 0)
	{
		kill_forked_pids(shell, SIGINT);
		ft_dprintf(2, "%s: %s: Unable to fork\n", ERR_PREFIX
				, pipe->cmd_inf->arg_lst->content);
	}
	else
	{
		setup_pipe_redirections(curr);
		exec_pipe(shell, pipe);
		exit(127);
	}
	return (pid);
}

static void		setup_pipes(t_shell *shell, t_list *pipeline)
{
	t_pipe	*cur_pipe;
	t_pipe	*next_pipe;

	while (pipeline != NULL)
	{
		cur_pipe = (t_pipe *)pipeline->content;
		if (pipeline->next != NULL)
		{
			next_pipe = (t_pipe *)pipeline->next->content;
			if (pipe(next_pipe->fildes) == 0)
			{
				cur_pipe->out_fd = (next_pipe->fildes)[1];
				next_pipe->in_fd = (next_pipe->fildes)[0];
			}
		}
		if (cur_pipe->cmd_inf->arg_lst != NULL
				&& pipe_fork(shell, pipeline) < 0)
			break ;
		if (!(cur_pipe->is_leftmost))
		{
			close((cur_pipe->fildes)[0]);
			close((cur_pipe->fildes)[1]);
		}
		pipeline = pipeline->next;
	}
}

void			execute_pipeline(t_shell *shell, t_cmd_inf *cmd_inf)
{
	t_list	*pipeline;

	pipeline = create_pipeline(cmd_inf);
	if (pipeline == NULL)
	{
		ft_dprintf(2, "%s: Unable to init pipeline\n", ERR_PREFIX);
		return ;
	}
	setup_pipes(shell, pipeline);
	ft_lstfree(&pipeline);
	if (shell->fork_pids == NULL)
		shell->last_status = 127;
	else
		wait_for_pipe_end(shell);
}
