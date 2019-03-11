/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/25 14:20:19 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 21:06:46 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "get_next_line.h"
#include "shell.h"
#include "input.h"
#include "redirect_inf.h"

static int	read_heredoc_line(t_shell *shell, t_redirect_inf *redirect_inf)
{
	int	ret;

	reset_cmdline(shell, PROMPT_HEREDOC);
	ret = read_input(shell);
	if (ret >= 0)
	{
		redirect_inf->heredoc = get_command_line(&(shell->term));
		ft_strdel(&(shell->term.prev_lines));
		if (ret != 0 && ft_strequ(shell->term.line, redirect_inf->to_word))
			return (0);
		shell->term.prev_lines = redirect_inf->heredoc;
	}
	if (ret == 0)
		ret = -1;
	return (ret);
}

void		prompt_heredoc(t_shell *shell, t_redirect_inf *redirect_inf)
{
	int	ret;

	ret = 1;
	setup_term(shell);
	while (ret > 0)
		ret = read_heredoc_line(shell, redirect_inf);
	reset_term(shell);
	shell->term.prev_lines = NULL;
	if (redirect_inf->heredoc == NULL)
		ft_dprintf(2, "%s: Unexpected error while reading heredoc\n"
				, ERR_PREFIX);
	else if (ret == 0)
		(redirect_inf->heredoc)[ft_strlen(redirect_inf->heredoc)
			- ft_strlen(redirect_inf->to_word)] = '\0';
	shell->term.prompt = PROMPT_DEF;
}

int			redirect_heredoc(t_redirect_inf *redirect_inf)
{
	int	fildes[2];

	if (redirect_inf->heredoc == NULL)
		return (0);
	if (pipe(fildes) < 0)
	{
		ft_dprintf(2, "%s: heredoc: Pipe error\n", ERR_PREFIX);
		return (0);
	}
	dup2(fildes[0], STDIN_FILENO);
	write(fildes[1], redirect_inf->heredoc
			, ft_strlen(redirect_inf->heredoc));
	close(fildes[1]);
	return (1);
}
