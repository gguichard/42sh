/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grammar.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/11 14:26:58 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/25 14:17:27 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "libft.h"
#include "lexer.h"

static int	expect_ope_right(int is_redir, t_list **curr)
{
	if (is_redir)
	{
		if (accept_token(curr, TK_STR_OPT))
			return (1);
		return (expect_token(curr, TK_WORD));
	}
	else
	{
		if (*curr == NULL)
			return (1);
		if (!is_token(curr, TK_CMD_SEP)
				&& !ft_strequ("|", ((t_token_inf *)(*curr)->content)->token))
			return (1);
		return (expect_token(curr, TK_WORD));
	}
}

static int	accept_operator(t_list **curr)
{
	int	is_redir;

	if (accept_token(curr, TK_NUM_OPT))
	{
		is_redir = is_redirection(*curr);
		return (!expect_token(curr, TK_OPE)
				|| !expect_ope_right(is_redir, curr) ? -1 : 1);
	}
	else
	{
		is_redir = is_redirection(*curr);
		if (accept_token(curr, TK_OPE))
			return (!expect_ope_right(is_redir, curr) ? -1 : 1);
	}
	return (0);
}

static int	lex_command_part(t_list **curr, int is_beginning)
{
	int	word_before;

	word_before = 0;
	while (accept_token(curr, TK_WORD))
		word_before = 1;
	if (*curr == NULL || is_token(curr, TK_CMD_SEP))
		return (0);
	if (is_beginning && !word_before
			&& ft_strequ("|", ((t_token_inf *)(*curr)->content)->token))
	{
		expect_token(curr, TK_WORD);
		return (-1);
	}
	return (accept_operator(curr));
}

static int	lex_command(t_list **curr)
{
	int	ret;
	int	is_beginning;

	is_beginning = 1;
	while ((ret = lex_command_part(curr, is_beginning)) > 0)
		is_beginning = 0;
	if (ret < 0)
		return (-1);
	return (*curr != NULL);
}

int			lex_commands(t_list *tokens)
{
	t_list	*curr;
	int		ret;

	curr = tokens;
	ret = 0;
	while ((ret = lex_command(&curr)) > 0)
	{
		if (!accept_token(&curr, TK_CMD_SEP))
			return (0);
	}
	if (ret < 0)
		return (0);
	if (curr != NULL)
	{
		ft_dprintf(2, "%s: syntax error near unexpected token `newline'\n"
				, ERR_PREFIX);
		return (0);
	}
	return (1);
}
