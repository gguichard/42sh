/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:47:19 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/12 19:33:24 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "cmdline.h"

void	print_prompt(t_cmdline *cmdline)
{
	if (cmdline->prompt_type == PROMPT_DEFAULT)
		ft_printf("prompt> ");
	else if (cmdline->prompt_type == PROMPT_BACKSLASH)
		ft_printf("> ");
	else if (cmdline->prompt_type == PROMPT_QUOTE)
		ft_printf("quote> ");
	else if (cmdline->prompt_type == PROMPT_DQUOTE)
		ft_printf("dquote> ");
	else if (cmdline->prompt_type == PROMPT_BRACKET)
		ft_printf("bracket> ");
	else if (cmdline->prompt_type == PROMPT_HEREDOC)
		ft_printf("heredoc> ");
}
