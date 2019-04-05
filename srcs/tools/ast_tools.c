#include "shell.h"
#include "parser_lexer.h"
#include "token_inf.h"
#include "error.h"

t_ast	*fill_cmd(t_list **lst_tk, t_ast *elem)
{
	t_list	*tmp;
	int		len;
	int		i;

	if (!elem)
		return (0);
	tmp = (*lst_tk)->next;
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
	if (!elem->input)
	{
		ft_memdel((void **)&elem);
		return (0);
	}
	*lst_tk = (*lst_tk)->next;
	tmp = *lst_tk;
	while (i < len)
	{
		if (get_tk(tmp)->type == TK_PARAM)
			if (!(elem->input[i++] = ft_strdup(get_tk(tmp)->token)))
			{
				del_elem_ast(&elem);
				return (0);
			}
		tmp = tmp->next;
	}
	return (elem);
}

t_ast	*fill_ope(t_list **lst_tk, t_ast *elem)
{
	int				len;
	int				i;
	t_token_type	type;

	if (!elem)
		return (0);
	i = 1;
	len = 0;
	type = get_tk(*lst_tk)->type;
	if (type == TK_RED_OPE)
		len = 2;
	else if (type == TK_RED_LOPT_FD)
		len = 3;
	init_input(elem, len, *lst_tk);
	if (!elem->input)
	{
		del_elem_ast(&elem);
		return (0);
	}
	*lst_tk = (*lst_tk)->next;
	while (i < len)
	{
		if (get_tk(*lst_tk)->token == NULL
				|| !(elem->input[i] = ft_strdup(get_tk(*lst_tk)->token)))
		{
			elem->input[i] = NULL;
			del_elem_ast(&elem);
			return (0);
		}
		i += 1;
		*lst_tk = (*lst_tk)->next;
	}
	return (elem);
}

t_ast	*fill_assign(t_list **lst_tk, t_ast *elem)
{
	t_list	*tmp;
	int		len;
	int		i;

	if (!elem)
		return (0);
	tmp = (*lst_tk)->next;
	len = 1;
	i = 1;
	while (tmp && get_tk(tmp)->type == TK_ASSIGN)
	{
		len += 1;
		tmp = tmp->next;
	}
	init_input(elem, len, *lst_tk);
	if (!elem->input)
	{
		ft_memdel((void **)&elem);
		return (0);
	}
	*lst_tk = (*lst_tk)->next;
	while (i < len)
	{
		if (!(elem->input[i] = ft_strdup(get_tk(*lst_tk)->token)))
		{
			del_elem_ast(&elem);
			return (0);
		}
		i += 1;
		*lst_tk = (*lst_tk)->next;
	}
	return (elem);
}

t_ast	*fill_cmd_sep(t_list **lst_tk, t_ast *elem)
{
	if (!elem)
		return (0);
	init_input(elem, 1, *lst_tk);
	if (!elem->input)
	{
		ft_memdel((void **)&elem);
		return (0);
	}
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
		elem = fill_cmd(lst_tk, set_new_elem());
	else if (type == TK_RED_LOPT_FD || type == TK_RED_OPE)
		elem = fill_ope(lst_tk, set_new_elem());
	else if (type == TK_CMD_SEP)
		elem = fill_cmd_sep(lst_tk, set_new_elem());
	else if (type == TK_ASSIGN)
		elem = fill_assign(lst_tk, set_new_elem());
	return (elem);
}
