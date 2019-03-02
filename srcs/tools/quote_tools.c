/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_tools.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/01 11:49:52 by tcollard          #+#    #+#             */
/*   Updated: 2018/12/13 18:17:17 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void	check_quote(char *s)
{
	int		i;
	char	*str;

	i = 0;
	str = NULL;
	while (s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
		{
			if (i != 0)
				str = ft_strsub(s, 0, i - 1);
			replace_quote(s, &i);
		}
		i += 1;
	}
}

void	replace_quote(char *s, int *i)
{
	char	quote;
	char	*new;
	char	*extract;
	int		j;

	new = NULL;
	extract = NULL;
	j = *i + 1;
	quote = s[*i];
	while (s[*i] != quote)
		*i += 1;
	extract = ft_strsub(s, j, *i - j + 1);
}
