#include "shell.h"
#include "parser_lexer.h"
#include "token_inf.h"

t_ast	*fill_cmd(t_list **lst_tk)
{
	t_list	*tmp;
	t_ast	*elem;
	int		len;
	int		i;

	tmp = (*lst_tk)->next;
	elem = set_new_elem();
	len = 1;
	i = 1;
	while (tmp && get_tk(tmp)->type != TK_CMD_SEP &&
	get_tk(tmp)->type != TK_CMD)
	{
		if (get_tk(tmp)->type == TK_PARAM)
			len += 1;
		tmp = tmp->next;
	}
	init_input(elem, len, *lst_tk);
	*lst_tk = (*lst_tk)->next;
	tmp = *lst_tk;
	while (i < len)
	{
		if (get_tk(tmp)->type == TK_PARAM)
		{
			if (!(elem->input[i] = ft_strdup(get_tk(tmp)->token)))
				ft_exit_malloc();
			i += 1;
		}
		tmp = tmp->next;
	}
	return (elem);
}

t_ast	*fill_ope(t_list **lst_tk)
{
	t_ast	*elem;
	int		len;
	int		i;

	elem = set_new_elem();
	i = 1;
	if (get_tk(*lst_tk)->type == TK_RED_OPE)
		len = 2;
	else
		len = 3;
	init_input(elem, len, *lst_tk);
	*lst_tk = (*lst_tk)->next;
	while (i < len)
	{
		if (!(elem->input[i] = ft_strdup(get_tk(*lst_tk)->token)))
			ft_exit_malloc();
		i += 1;
		*lst_tk = (*lst_tk)->next;
	}
	return (elem);
}

t_ast	*fill_assign(t_list **lst_tk)
{
	t_list	*tmp;
	t_ast	*elem;
	int		len;
	int		i;

	tmp = (*lst_tk)->next;
	elem = set_new_elem();
	len = 1;
	i = 1;
	while (tmp && get_tk(tmp)->type == TK_ASSIGN)
	{
		len += 1;
		tmp = tmp->next;
	}
	init_input(elem, len, *lst_tk);
	*lst_tk = (*lst_tk)->next;
	while (i < len)
	{
		if (!(elem->input[i] = ft_strdup(get_tk(*lst_tk)->token)))
			ft_exit_malloc();
		i += 1;
		*lst_tk = (*lst_tk)->next;
	}
	return (elem);
}

t_ast	*fill_cmd_sep(t_list **lst_tk)
{
	t_ast	*elem;

	elem = set_new_elem();
	init_input(elem, 1, *lst_tk);
	*lst_tk = (*lst_tk)->next;
	return (elem);
}

t_ast	*create_elem(t_list **lst_tk)
{
	t_ast			*elem;
	t_token_type	type;

	elem = NULL;
	type = get_tk(*lst_tk)->type;
	if (type == TK_CMD)
		elem = fill_cmd(lst_tk);
	else if (type == TK_LRED_OPT || type == TK_RED_OPE)
		elem = fill_ope(lst_tk);
	else if (type == TK_CMD_SEP)
		elem = fill_cmd_sep(lst_tk);
	else if (type == TK_ASSIGN)
		elem = fill_assign(lst_tk);
	return (elem);
}
