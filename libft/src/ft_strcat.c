/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcat.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/06 17:42:05 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/13 10:42:11 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strcat(char *dest, const char *src)
{
	if (!dest)
		return ((char *)src);
	else if (src)
		ft_memcpy(dest + ft_strlen(dest), src, ft_strlen(src) + 1);
	return (dest);
}
