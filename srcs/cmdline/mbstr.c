/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mbstr.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 11:28:19 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/22 15:58:21 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <wchar.h>
#include <inttypes.h>
#include "libft.h"

int		utf8_size(unsigned char first_byte)
{
	if ((first_byte & 0xF0) == 0xF0)
		return (4);
	else if ((first_byte & 0xE0) == 0xE0)
		return (3);
	else if ((first_byte & 0xC0) == 0xC0)
		return (2);
	else
		return (1);
}

wint_t	utf8_decode(uint32_t c, int len)
{
	wint_t	point;

	point = 0;
	if (len == 1)
		point = c & 0x7F;
	else if (len == 2)
	{
		point = c & 0x3F;
		point |= ((c >> 8) & 0x1F) << 6;
	}
	else if (len == 3)
	{
		point = c & 0x3F;
		point |= ((c >> 8) & 0x3F) << 6;
		point |= ((c >> 16) & 0xF) << 12;
	}
	else if (len == 4)
	{
		point = c & 0x3F;
		point |= ((c >> 8) & 0x3F) << 6;
		point |= ((c >> 16) & 0x3F) << 12;
		point |= ((c >> 24) & 0x7) << 18;
	}
	return (point);
}

int		utf8_encode(char *dest, wint_t c)
{
	int	size;

	size = 0;
	if (c < (1 << 7))
		dest[size++] = c;
	else if (c < (1 << 11))
	{
		dest[size++] = (c >> 6) | 0xC0;
		dest[size++] = (c & 0x3F) | 0x80;
	}
	else if (c < (1 << 16))
	{
		dest[size++] = (c >> 12) | 0xE0;
		dest[size++] = ((c >> 6) & 0x3F) | 0x80;
		dest[size++] = (c & 0x3F) | 0x80;
	}
	else if (c < (1 << 21))
	{
		dest[size++] = (c >> 18) | 0xF0;
		dest[size++] = ((c >> 12) & 0x3F) | 0x80;
		dest[size++] = ((c >> 6) & 0x3F) | 0x80;
		dest[size++] = (c & 0x3F) | 0x80;
	}
	return (size);
}

char		*wstr_to_mbstr(const wchar_t *wstr, size_t len)
{
	char	*str;
	size_t	offset;

	str = (char *)malloc(len * sizeof(wint_t) + 1);
	if (str == NULL)
		return (NULL);
	offset = 0;
	while (len > 0)
	{
		offset += utf8_encode(str + offset, *wstr);
		wstr++;
		len--;
	}
	str[offset] = '\0';
	return (str);
}
