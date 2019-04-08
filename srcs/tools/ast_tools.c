/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 00:37:29 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/06 00:37:31 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		return (NULL);
	tmp = (*lst_tk);
	len = 1;
	i = 1;
	while ((tmp = tmp->next) && get_tk(tmp)->type != TK_CMD_SEP)
		len += (get_tk(tmp)->type == TK_PARAM) ? 1 : 0;
	if (!init_input(elem, len, *lst_tk))
		return (ft_memdel((void **)&elem));
	*lst_tk = (*lst_tk)->next;
	tmp = *lst_tk;
	while (i < len)
	{
		if (get_tk(tmp)->type == TK_PARAM)
			if (!(elem->input[i++] = ft_strdup(get_tk(tmp)->token)))
				return (del_elem_ast(&elem));
		tmp = tmp->next;
	}
	return (elem);
}

t_ast	*fill_ope(t_list **lst_tk, t_ast *elem)
{
	int				len;
	int				i;

	if (!elem)
		return (NULL);
	i = 1;
	len = (get_tk(*lst_tk)->type == TK_RED_OPE) ? 2 : 3;
	if (!init_input(elem, len, *lst_tk))
		return (ft_memdel((void **)&elem));
	*lst_tk = (*lst_tk)->next;
	while (i < len)
	{
		if (get_tk(*lst_tk)->token == NULL
				|| !(elem->input[i] = ft_strdup(get_tk(*lst_tk)->token)))
		{
			elem->input[i] = NULL;
			return (del_elem_ast(&elem));
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
		return (NULL);
	tmp = (*lst_tk)->next;
	len = 1;
	i = 1;
	while (tmp && get_tk(tmp)->type == TK_ASSIGN)
	{
		len += 1;
		tmp = tmp->next;
	}
	if (!init_input(elem, len, *lst_tk))
		return (ft_memdel((void **)&elem));
	*lst_tk = (*lst_tk)->next;
	while (i < len)
	{
		if (!(elem->input[i] = ft_strdup(get_tk(*lst_tk)->token)))
			return (del_elem_ast(&elem));
		i += 1;
		*lst_tk = (*lst_tk)->next;
	}
	return (elem);
}

t_ast	*fill_cmd_sep(t_list **lst_tk, t_ast *elem)
{
	if (!elem)
		return (NULL);
	if (!init_input(elem, 1, *lst_tk))
		return (ft_memdel((void **)&elem));
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
