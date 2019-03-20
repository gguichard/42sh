#include "shell.h"
#include "parser_lexer.h"
#include "token_inf.h"

static int	syntax_error(t_list *lst_tk)
{
	ft_dprintf(2, "42sh: syntax error near unexpected token `%s'\n",
	get_tk(lst_tk)->token);
	return (0);
}

int	token_analyser(t_list *lst_tk)
{
	if (get_tk(lst_tk)->type == TK_CMD_SEP)
		return (syntax_error(lst_tk));
	while (lst_tk->next)
	{
		if (get_tk(lst_tk)->type == TK_CMD_SEP
		&& get_tk(lst_tk->next)->type == TK_CMD_SEP)
			return (syntax_error(lst_tk->next));
		else if (get_tk(lst_tk)->type == TK_RED_OPE
		&& get_tk(lst_tk->next)->type == TK_CMD_SEP)
			return (syntax_error(lst_tk->next));
		lst_tk = lst_tk->next;
	}
	if (get_tk(lst_tk)->type == TK_CMD_SEP
	&& ft_strcmp(get_tk(lst_tk)->token, ";") != 0)
		ft_printf("recall pompt for %s\n", get_tk(lst_tk)->token);
	return (1);
}
