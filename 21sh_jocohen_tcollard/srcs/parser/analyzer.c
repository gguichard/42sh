/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyzer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/01 11:48:41 by tcollard          #+#    #+#             */
/*   Updated: 2019/03/01 19:24:24 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	analyzer(t_ast *sort, t_env **lst_env, t_alloc **alloc)
{
	static t_dispatch	dispatch[] = { &dispatch_cmd, &dispatch_redir,
						&dispatch_redir, &dispatch_agreg, &dispatch_operator,
						&dispatch_logic };
	char				**tab_path;
	t_ast				*tmp;
	int					ret;

	ret = 0;
	tmp = sort;
	tab_path = NULL;
	if (tmp && tmp->print == 0)
	{
		tmp->print = 1;
		return (dispatch[tmp->type](tmp, lst_env, tab_path, alloc));
	}
	if (tmp && tmp->left && tmp->left->print == 0)
		return (analyzer(tmp->left, lst_env, alloc));
	else if (tmp && tmp->right && tmp->right->print == 0)
		return (analyzer(tmp->right, lst_env, alloc));
	else if (tmp && tmp->back)
		return (analyzer(tmp->back, lst_env, alloc));
	return (ret);
}
