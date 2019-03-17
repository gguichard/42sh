/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_wstrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/17 16:49:50 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/17 16:50:07 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <wchar.h>
#include "libft.h"

wchar_t	*ft_wstrchr(const wchar_t *str, wint_t c)
{
	while (*str != L'\0')
	{
		if (*str == c)
			return ((wchar_t *)str);
		str++;
	}
	return (NULL);
}
