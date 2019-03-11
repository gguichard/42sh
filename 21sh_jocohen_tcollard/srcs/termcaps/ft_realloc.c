/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/24 11:41:58 by jocohen           #+#    #+#             */
/*   Updated: 2019/02/21 16:00:02 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	*ft_realloc(void *ptr, size_t size, size_t len)
{
	void	*tmp;

	if (!(tmp = ft_memalloc(size)))
		ft_exit_malloc();
	ft_memcpy(tmp, ptr, len);
	ft_memdel((void **)&ptr);
	return ((void *)tmp);
}
