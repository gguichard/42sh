/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mbstr.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 11:28:19 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/21 11:33:11 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <wchar.h>
#include "libft.h"

char	*wstr_to_mbstr(const wchar_t *wstr, size_t len)
{
	char	*str;
	size_t	offset;
	int		wlen;

	str = (char *)malloc(len * sizeof(wint_t));
	if (str == NULL)
		return (NULL);
	offset = 0;
	while (len > 0)
	{
		wlen = ft_wcharlen(*wstr);
		ft_memcpy(str + offset, wstr, wlen);
		offset += wlen;
		wstr++;
		len--;
	}
	str[offset] = '\0';
	return (str);
}
