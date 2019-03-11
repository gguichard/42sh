/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/29 15:06:03 by gguichar          #+#    #+#             */
/*   Updated: 2018/09/03 02:03:06 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_memory_offset(const char *s)
{
	size_t	offset;

	offset = 0;
	while (((unsigned long)s & (sizeof(t_lword) - 1)) != 0)
	{
		if (*s == '\0')
			return (offset);
		offset++;
		s++;
	}
	return (offset);
}

static size_t	ft_word_offset(const char *s)
{
	if (s[0] == '\0')
		return (0);
	if (s[1] == '\0')
		return (1);
	if (s[2] == '\0')
		return (2);
	if (s[3] == '\0')
		return (3);
	if (sizeof(t_lword) > 4)
	{
		if (s[4] == '\0')
			return (4);
		if (s[5] == '\0')
			return (5);
		if (s[6] == '\0')
			return (6);
		if (s[7] == '\0')
			return (7);
	}
	return (404);
}

size_t			ft_strlen(const char *s)
{
	t_lword	*src;
	t_lword	himagic;
	t_lword	lomagic;
	t_lword	value;
	size_t	offset;

	src = (t_lword *)(s + ft_memory_offset(s));
	if (*src == '\0')
		return ((const char *)src - s);
	himagic = 0x80808080L;
	lomagic = 0x01010101L;
	if (sizeof(value) > 4)
	{
		himagic |= (himagic << 16) << 16;
		lomagic |= (lomagic << 16) << 16;
	}
	while (1)
	{
		value = *src;
		if (((value - lomagic) & ~value & himagic) != 0
			&& (offset = ft_word_offset((const char *)src)) != 404)
			return ((const char *)src - s + offset);
		src++;
	}
}
