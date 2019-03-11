/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wcharlen.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/12/10 10:30:49 by gguichar          #+#    #+#             */
/*   Updated: 2018/12/10 10:31:54 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_wcharlen(wint_t c)
{
	if (c < (1 << 7))
		return (1);
	else if (c < (1 << 11))
		return (2);
	else if (c < (1 << 16))
		return (3);
	else if (c < (1 << 21))
		return (4);
	return (-1);
}
