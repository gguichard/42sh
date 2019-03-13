/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:47:19 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/13 09:56:29 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "cmdline.h"

void	print_prompt(t_cmdline *cmdline)
{
	if (cmdline->prompt.type == PROMPT_DEFAULT)
		cmdline->prompt.offset = ft_printf("prompt> ");
	else if (cmdline->prompt.type == PROMPT_BACKSLASH)
		cmdline->prompt.offset = ft_printf("> ");
	else if (cmdline->prompt.type == PROMPT_QUOTE)
		cmdline->prompt.offset = ft_printf("quote> ");
	else if (cmdline->prompt.type == PROMPT_DQUOTE)
		cmdline->prompt.offset = ft_printf("dquote> ");
	else if (cmdline->prompt.type == PROMPT_BRACKET)
		cmdline->prompt.offset = ft_printf("bracket> ");
	else if (cmdline->prompt.type == PROMPT_HEREDOC)
		cmdline->prompt.offset = ft_printf("heredoc> ");
}
