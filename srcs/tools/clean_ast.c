/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 23:47:25 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/06 00:33:57 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "shell.h"

void	*del_elem_ast(t_ast **lst)
{
	if (lst == NULL || *lst == NULL)
		return (NULL);
	ft_strtab_free((*lst)->input);
	return (ft_memdel((void **)lst));
}

void	del_ast(t_ast **lst)
{
	if (lst == NULL || *lst == NULL)
		return ;
	if ((*lst)->left != NULL)
		del_ast(&(*lst)->left);
	if ((*lst)->right != NULL)
		del_ast(&(*lst)->right);
	del_elem_ast(lst);
}
