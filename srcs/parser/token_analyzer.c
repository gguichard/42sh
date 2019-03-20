#include "shell.h"
#include "parser_lexer.h"
#include "token_inf.h"

static t_recall_prompt	syntax_error(const char *tk_error)
{
	ft_dprintf(2, "42sh: syntax error near unexpected token `%s'\n", tk_error);
	return (PR_ERROR);
}

static t_recall_prompt	recall_prompt_type(t_list *lst_tk)
{
	if (ft_strcmp(get_tk(lst_tk)->token, "|") == 0)
		return (PR_PIPE);
	else if (ft_strcmp(get_tk(lst_tk)->token, "||") == 0)
		return (PR_OR);
	else if (ft_strcmp(get_tk(lst_tk)->token, "&&") == 0)
		return (PR_AND);
	return (PR_ERROR);
}

t_recall_prompt			token_analyser(t_list *lst_tk)
{
	if (get_tk(lst_tk)->type == TK_CMD_SEP)
		return (syntax_error(get_tk(lst_tk)->token));
	while (lst_tk && get_tk(lst_tk)->type == TK_ASSIGN)
		lst_tk = lst_tk->next;
	while (lst_tk && lst_tk->next)
	{
		if (get_tk(lst_tk)->type == TK_CMD_SEP
		&& get_tk(lst_tk->next)->type == TK_CMD_SEP)
			return (syntax_error(get_tk(lst_tk->next)->token));
		else if (get_tk(lst_tk)->type == TK_RED_OPE
		&& get_tk(lst_tk->next)->type == TK_CMD_SEP)
			return (syntax_error(get_tk(lst_tk->next)->token));
		lst_tk = lst_tk->next;
	}
	if (get_tk(lst_tk)->type == TK_CMD_SEP
	&& ft_strcmp(get_tk(lst_tk)->token, ";") != 0)
		return (recall_prompt_type(lst_tk));
	else if (!(lst_tk->next) && get_tk(lst_tk)->type == TK_RED_OPE)
		return (syntax_error("newline"));
	return (PR_SUCCESS);
}
