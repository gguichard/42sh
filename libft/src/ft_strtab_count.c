/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtab_count.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/20 20:03:07 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/20 20:03:37 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strtab_count(char **tab)
{
	size_t	idx;

	idx = 0;
	while (tab[idx] != NULL)
		idx++;
	return (idx);
}
