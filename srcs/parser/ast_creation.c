/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_creation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 19:57:05 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/06 00:18:31 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "shell.h"
#include "parser_lexer.h"
#include "token_inf.h"
#include "error.h"

t_ast	*create_ast_branch(t_list **lst_tk)
{
	t_ast			*sort;
	t_ast			*elem;
	t_token_type	type;

	sort = NULL;
	elem = NULL;
	while (*lst_tk)
	{
		type = get_tk(*lst_tk)->type;
		if (type == TK_CMD_SEP && ft_strcmp(get_tk(*lst_tk)->token, ";") == 0)
			break ;
		else if (type != TK_PARAM)
		{
			if (!(elem = create_elem(lst_tk)))
			{
				del_ast(&sort);
				return (NULL);
			}
			sort_ast(elem, &sort);
		}
		else
			*lst_tk = (*lst_tk)->next;
	}
	return (sort);
}

int		link_branch(t_ast **elem, t_ast **branch, t_ast **sort
	, t_list **lst_tk)
{
	if (!(*sort))
		*sort = *branch;
	if (*sort && (*sort)->type == AST_CMD_SEP && !(*sort)->right)
	{
		(*sort)->right = *branch;
		(*branch)->back = *sort;
	}
	else if (*lst_tk != NULL && get_tk(*lst_tk)->type == TK_CMD_SEP)
	{
		if (!(*elem = create_elem(lst_tk)))
		{
			del_ast(sort);
			del_ast(branch);
			return (0);
		}
		(*elem)->left = *sort;
		(*sort)->back = *elem;
		*sort = *elem;
	}
	else if (*lst_tk != NULL)
		*lst_tk = (*lst_tk)->next;
	return (1);
}
