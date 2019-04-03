/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:47:19 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/03 14:24:57 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "shell.h"
#include "cmdline.h"

char	*get_prompt(t_cmdline *cmdline, t_prompt type)
{
	(void)cmdline;
	if (type == PROMPT_DEFAULT)
		return (create_prompt(cmdline->alloc->vars));
	else if (type == PROMPT_OPERATOR)
		return (ft_strdup("ope> "));
	else if (type == PROMPT_QUOTE)
		return (ft_strdup("quote> "));
	else if (type == PROMPT_DQUOTE)
		return (ft_strdup("dquote> "));
	else if (type == PROMPT_BRACKET)
		return (ft_strdup("bracket> "));
	else if (type == PROMPT_HEREDOC)
		return (ft_strdup("heredoc> "));
	else
		return (ft_strdup("> "));
}
