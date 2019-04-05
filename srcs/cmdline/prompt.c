/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:47:19 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/05 15:45:26 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"
#include "libft.h"
#include "shell.h"
#include "cmdline.h"

char		*get_prompt(t_cmdline *cmdline, t_prompt type, size_t *offset)
{
	char	*str;

	if (type == PROMPT_DEFAULT)
		return (create_prompt(cmdline->alloc->vars, offset));
	else if (type == PROMPT_OPERATOR)
		str = ft_strdup("operator> ");
	else if (type == PROMPT_QUOTE)
		str = ft_strdup("quote> ");
	else if (type == PROMPT_DQUOTE)
		str = ft_strdup("dquote> ");
	else if (type == PROMPT_BRACKET)
		str = ft_strdup("bracket> ");
	else if (type == PROMPT_HEREDOC)
		str = ft_strdup("heredoc> ");
	else
		str = ft_strdup("> ");
	*offset = (str == NULL ? 0 : ft_strlen(str));
	return (str);
}

static char	*non_interact_input(t_cmdline *cmdline, t_rstate *state)
{
	char	*str;
	int		ret;

	(void)cmdline;
	ret = get_next_line(STDIN_FILENO, &str);
	*state = (ret <= 0 ? RSTATE_EOT : RSTATE_END);
	return (str);
}

char		*create_prompt_and_read_input(t_cmdline *cmdline, t_prompt type
		, t_rstate *state)
{
	char	*prompt;
	size_t	offset;

	if (!isatty(STDIN_FILENO))
		return (non_interact_input(cmdline, state));
	prompt = get_prompt(cmdline, type, &offset);
	if (prompt == NULL)
		*state = read_input(cmdline, "> ", 2);
	else
	{
		*state = read_input(cmdline, prompt, offset);
		free(prompt);
	}
	return (ft_strdup(cmdline->input.buffer));
}
