/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 23:47:08 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/05 23:47:10 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "hashtable.h"
#include "vars.h"

static void	del_lst_ast(t_ast **lst)
{
	t_ast	*tmp;

	tmp = *lst;
	if (lst == NULL || tmp == NULL)
		return ;
	*lst = (*lst)->back;
	ft_strtab_free(tmp->input);
	free(tmp);
}

void		del_alloc(t_alloc *alloc)
{
	if (alloc == NULL)
		return ;
	if (alloc->vars != NULL)
		ft_lstdel(&alloc->vars, free_var);
	if (alloc->exectable != NULL)
		delete_hashtable(alloc->exectable);
	alloc->exectable = NULL;
	if (alloc->aliastable != NULL)
		delete_hashtable(alloc->aliastable);
	alloc->aliastable = NULL;
	if (alloc->ast)
	{
		del_lst_ast(&alloc->ast);
		alloc->ast = NULL;
	}
	alloc = NULL;
}
