/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strreplace_inside.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/22 16:12:02 by fwerner           #+#    #+#             */
/*   Updated: 2019/03/22 16:25:33 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

int		ft_strreplace_inside(char **str, size_t pos, size_t len
		, const char *replacement)
{
	size_t	str_len;
	size_t	replacement_len;
	char	*new_str;

	str_len = ft_strlen(*str);
	replacement_len = ft_strlen(replacement);
	if ((new_str = (char*)malloc(str_len - len + replacement_len + 1)) == NULL)
		return (0);
	ft_memcpy(new_str, *str, pos);
	ft_memcpy(new_str + pos, replacement, replacement_len);
	ft_memcpy(new_str + pos + replacement_len, *str + pos + len
			, str_len - pos - len);
	new_str[str_len - len + replacement_len] = '\0';
	free(*str);
	*str = new_str;
	return (1);
}
