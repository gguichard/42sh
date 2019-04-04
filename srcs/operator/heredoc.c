/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/27 14:51:18 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/04 19:05:28 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "job.h"
#include "parser_lexer.h"
#include "builtins.h"
#include "cmdline.h"

static char	*join_heredoc(char *heredoc, char *part)
{
	char	*tab[3];
	char	*ret;

	tab[0] = heredoc;
	tab[1] = part;
	tab[2] = NULL;
	ret = ft_join(tab, "\n");
	free(heredoc);
	return (ret);
}

char		*read_heredoc(t_cmdline *cmdline, const char *word)
{
	char		*heredoc;
	t_rstate	state;

	heredoc = ft_strdup("");
	if (heredoc == NULL)
		return (NULL);
	setup_term(cmdline);
	while (1)
	{
		setup_term(cmdline);
		while (1)
		{
			state = read_input(cmdline, prompt, offset);
			if (state == RSTATE_ETX)
				continue ;
			if (state != RSTATE_END || ft_strequ(cmdline->input.buffer, word))
				break ;
			heredoc = join_heredoc(heredoc, cmdline->input.buffer);
			if (heredoc == NULL)
				break ;
		}
		reset_term(cmdline);
	}
	reset_term(cmdline);
	return (heredoc);
}

int			heredoc(t_ast *elem, t_alloc *alloc, t_exec_opt *opt)
{
	pid_t	pid;
	int		fildes[2];
	char	*heredoc;

	// TODO: skip prochains heredoc
	if (!opt->fork)
	{
		pid = fork();
		if (pid == -1)
			return (2);
		else if (pid > 0)
		{
			wait_pid(pid, elem->left, opt, alloc);
			return (ret_status(alloc->ret_val, pid, 0));
		}
	}
	opt->fork = true;
	if (pipe(fildes) != -1)
	{
		dup2(fildes[0], STDIN_FILENO);
		heredoc = "test de heredoc\n";
		write(fildes[1], heredoc, ft_strlen(heredoc));
		close(fildes[1]);
		close(fildes[0]);
	}
	return (analyzer(elem->left, alloc, opt));
}
