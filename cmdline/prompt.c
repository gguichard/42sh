/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:47:19 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/18 20:28:01 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "cmdline.h"

const char	*get_prompt(t_cmdline *cmdline)
{
	if (cmdline->prompt.type == PROMPT_DEFAULT)
		return ("prompt> ");
	else if (cmdline->prompt.type == PROMPT_BACKSLASH)
		return ("> ");
	else if (cmdline->prompt.type == PROMPT_QUOTE)
		return ("quote> ");
	else if (cmdline->prompt.type == PROMPT_DQUOTE)
		return ("dquote> ");
	else if (cmdline->prompt.type == PROMPT_BRACKET)
		return ("bracket> ");
	else if (cmdline->prompt.type == PROMPT_HEREDOC)
		return ("heredoc> ");
	else
		return ("?> ");
}
