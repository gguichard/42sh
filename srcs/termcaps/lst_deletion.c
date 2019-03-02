/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lst_deletion.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/25 17:14:04 by jocohen           #+#    #+#             */
/*   Updated: 2018/12/13 16:33:47 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		ft_error(void **a1, void **a2, void **a3, void **a4)
{
	ft_memdel(a1);
	ft_memdel(a2);
	ft_memdel(a3);
	ft_memdel(a4);
	return (0);
}

int		lst_deletion(t_env **fp)
{
	t_env	*tmp;

	while ((*fp))
	{
		tmp = (*fp)->next;
		ft_memdel((void **)&((*fp)->content));
		(*fp)->content_size = 0;
		ft_memdel((void **)fp);
		(*fp) = tmp;
	}
	return (0);
}
