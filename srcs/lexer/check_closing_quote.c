/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_closing_quote.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/01 11:25:41 by tcollard          #+#    #+#             */
/*   Updated: 2019/02/21 20:58:14 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		check_opening_quote(char **str, t_alloc *alloc)
{
	int	i;

	i = 0;
	if (!(*str))
		return (1);
	while ((*str)[i])
	{
		if (ft_isquote((*str)[i]) == 1)
			if (!find_closing(str, &i, alloc))
				return (0);
		i += 1;
	}
	return (1);
}

int		find_closing(char **str, int *i, t_alloc *alloc)
{
	char	c;
	char	*tmp;
	char	*save;

	tmp = NULL;
	save = NULL;
	c = (*str)[*i];
	*i += 1;
	while ((*str)[*i] && (*str)[*i] != c)
		*i += 1;
	if ((*str)[*i] == c)
		return (1);
	else
	{
		if (!(tmp = missing_quote_prompt(c, alloc)))
			return (0);
		(!(save = ft_strdup(*str))) ? ft_exit_malloc() : 0;
		free(*str);
		if (!(*str = ft_strjoin(save, tmp)))
			ft_exit_malloc();
		free(save);
		free(tmp);
		*i = ft_strlen(*str);
	}
	return (1);
}
