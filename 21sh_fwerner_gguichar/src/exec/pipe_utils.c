/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 12:04:35 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 20:53:36 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "libft.h"
#include "execute.h"

void	setup_pipe_redirections(t_list *cur)
{
	t_pipe	*pipe;

	pipe = (t_pipe *)cur->content;
	if (!(pipe->is_leftmost) && pipe->in_fd != -1)
	{
		dup2(pipe->in_fd, STDIN_FILENO);
		close(pipe->in_fd);
		close((pipe->fildes)[1]);
	}
	if (!(pipe->is_rightmost) && pipe->out_fd != -1)
	{
		dup2(pipe->out_fd, STDOUT_FILENO);
		close(pipe->out_fd);
		close((((t_pipe *)cur->next->content)->fildes)[0]);
	}
}

void	wait_for_pipe_end(t_shell *shell)
{
	t_list	*curr;
	t_list	*next;
	int		status;

	curr = shell->fork_pids;
	while (curr != NULL)
	{
		waitpid(*((pid_t *)curr->content), &status, 0);
		if (WIFSIGNALED(status))
			print_sig_message(status);
		next = curr->next;
		free(curr->content);
		free(curr);
		curr = next;
	}
	shell->fork_pids = NULL;
	shell->last_status = WEXITSTATUS(status);
}
