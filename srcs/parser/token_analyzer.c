/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_analyzer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 18:05:44 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/09 14:20:37 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "cmdline.h"
#include "parser_lexer.h"
#include "token_inf.h"

static t_recall_prompt	syntax_error(t_token_inf *tk_error)
{
	if (tk_error != NULL)
		ft_dprintf(STDERR_FILENO, "42sh: "
				"syntax error near unexpected token `%s'\n", tk_error->token);
	else
		ft_dprintf(STDERR_FILENO, "42sh: "
				"syntax error near unexpected token `newline'\n");
	return (PR_ERROR);
}

static t_recall_prompt	recall_prompt_type(t_list *lst_tk)
{
	if (ft_strequ(get_tk(lst_tk)->token, "|"))
		return (PR_PIPE);
	else if (ft_strequ(get_tk(lst_tk)->token, "||"))
		return (PR_OR);
	else if (ft_strequ(get_tk(lst_tk)->token, "&&"))
		return (PR_AND);
	return (PR_ERROR);
}

static int				check_red_ope(t_list **lst_tk, int prompt_hdc)
{
	char	*heredoc;

	if (prompt_hdc && get_tk(*lst_tk)->type == TK_RED_OPE
			&& ft_strequ(get_tk(*lst_tk)->token, "<<")
			&& (*lst_tk)->next != NULL
			&& get_tk((*lst_tk)->next)->type == TK_RED_ROPT_FILE)
	{
		heredoc = prompt_heredoc(g_cmdline, get_tk((*lst_tk)->next)->token);
		if (heredoc == NULL)
			return (0);
		free(get_tk((*lst_tk)->next)->token);
		get_tk((*lst_tk)->next)->token = heredoc;
	}
	*lst_tk = (*lst_tk)->next;
	if ((*lst_tk) == NULL || (get_tk(*lst_tk)->type != TK_RED_ROPT_FILE
				&& get_tk(*lst_tk)->type != TK_RED_ROPT_FILE))
		return (0);
	if (ft_strequ((get_tk(*lst_tk)->token), "&") && (*lst_tk)->next != NULL
			&& get_tk((*lst_tk)->next)->type != TK_RED_ROPT_FILE)
		return (0);
	return (1);
}

static int				check_lred_opt(t_list **lst_tk, int prompt_hdc)
{
	*lst_tk = (*lst_tk)->next;
	if ((*lst_tk) == NULL || get_tk(*lst_tk)->type != TK_RED_OPE)
		return (0);
	return (check_red_ope(lst_tk, prompt_hdc));
}

t_recall_prompt			token_analyser(t_list *lst_tk, int prompt_hdc)
{
	t_token_type	type;

	if (lst_tk && get_tk(lst_tk)->type == TK_CMD_SEP)
		return (syntax_error(get_tk(lst_tk)));
	while (lst_tk && get_tk(lst_tk)->type == TK_ASSIGN)
		lst_tk = lst_tk->next;
	while (lst_tk && lst_tk->next && (type = get_tk(lst_tk)->type))
	{
		if (type != TK_CMD_SEP && ft_strequ(get_tk(lst_tk)->token, ";"))
			break ;
		else if (type == TK_CMD_SEP && get_tk(lst_tk->next)->type == TK_CMD_SEP)
			return (syntax_error(get_tk(lst_tk->next)));
		else if (type == TK_RED_LOPT_FD && !check_lred_opt(&lst_tk, prompt_hdc))
			return (syntax_error(get_tk(lst_tk)));
		else if (type == TK_RED_OPE && !check_red_ope(&lst_tk, prompt_hdc))
			return (syntax_error(get_tk(lst_tk)));
		lst_tk = lst_tk->next;
	}
	if (lst_tk && get_tk(lst_tk)->type == TK_CMD_SEP
			&& !ft_strequ(get_tk(lst_tk)->token, ";")
			&& !ft_strequ(get_tk(lst_tk)->token, "&"))
		return (recall_prompt_type(lst_tk));
	else if (lst_tk && !(lst_tk->next) && get_tk(lst_tk)->type == TK_RED_OPE)
		return (syntax_error(NULL));
	return (lst_tk ? token_analyser(lst_tk->next, prompt_hdc) : PR_SUCCESS);
}
