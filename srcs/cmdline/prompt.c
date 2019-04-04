/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:47:19 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/03 19:02:51 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "shell.h"
#include "cmdline.h"

char	*get_prompt(t_cmdline *cmdline, t_prompt type, size_t *offset)
{
	char	*str;

	if (type == PROMPT_DEFAULT)
		return (create_prompt(cmdline->alloc->vars, offset));
	else if (type == PROMPT_OPERATOR)
		str = ft_strdup("ope> ");
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
