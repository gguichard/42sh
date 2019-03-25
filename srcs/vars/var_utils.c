/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/25 15:29:37 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/25 15:55:38 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	is_var_valid_identifier(const char *key)
{
	size_t	offset;

	if (ft_isdigit(key[0]))
		return (0);
	offset = 0;
	while (key[offset] != '\0')
	{
		if (!ft_isalnum(key[offset]) && key[offset] != '_')
			return (0);
		offset++;
	}
	return (1);
}
