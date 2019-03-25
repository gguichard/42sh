/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:47:19 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/21 17:19:19 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "cmdline.h"

const char	*get_prompt(t_cmdline *cmdline, t_prompt type)
{
	(void)cmdline;
	if (type == PROMPT_DEFAULT)
		return ("prompt> ");
	else if (type == PROMPT_OPERATOR)
		return ("ope> ");
	else if (type == PROMPT_QUOTE)
		return ("quote> ");
	else if (type == PROMPT_DQUOTE)
		return ("dquote> ");
	else if (type == PROMPT_BRACKET)
		return ("bracket> ");
	else if (type == PROMPT_HEREDOC)
		return ("heredoc> ");
	else
		return ("> ");
}
