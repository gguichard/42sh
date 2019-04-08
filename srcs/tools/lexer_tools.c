/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/06 00:41:16 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/06 00:41:18 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "error.h"

void	ft_insert(char **source, char *insert, int pos1, int pos2)
{
	char	*begin;
	char	*end;
	char	*tmp;

	begin = ft_strsub(*source, 0, pos1);
	end = ft_strsub(*source, pos2 + 1, (ft_strlen(&((*source)[pos2]) - 1)));
	ft_memdel((void **)source);
	if (!(tmp = ft_strjoin(begin, insert)))
		return ;
	if (!(*source = ft_strjoin(tmp, end)))
		return ;
	ft_memdel((void **)&begin);
	ft_memdel((void **)&tmp);
	ft_memdel((void **)&end);
}
