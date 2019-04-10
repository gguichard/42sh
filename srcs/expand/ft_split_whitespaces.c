/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_whitespaces.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/10 10:29:54 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/10 10:58:05 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "expand.h"

static size_t	ft_count_words(char const *s)
{
	size_t	len;
	size_t	words;
	int		is_space_flag;

	len = 0;
	words = 0;
	is_space_flag = 0;
	while (s[len] != '\0')
	{
		is_space_flag = ft_isspace(s[len]);
		if (!is_space_flag || len == 0)
			words++;
		while (s[len] != '\0' && is_space_flag == ft_isspace(s[len]))
			len++;
	}
	if (is_space_flag && s[len] == '\0' && (words > 1 || !ft_isspace(s[0])))
		words++;
	return (words);
}

static size_t	ft_word_len(const char *s)
{
	size_t	len;
	int		is_space_flag;

	len = 0;
	is_space_flag = ft_isspace(s[len]);
	while (s[len] != '\0' && is_space_flag == ft_isspace(s[len]))
		len++;
	return (len);
}

char			**ft_split_whitespaces(const char *s)
{
	size_t	words;
	char	**tab;
	size_t	i;
	size_t	word_len;

	if (s == NULL)
		return (NULL);
	words = ft_count_words(s);
	if (!(tab = (char **)malloc(sizeof(*tab) * (words + 1))))
		return (NULL);
	i = 0;
	while (i < words)
	{
		word_len = ft_word_len(s);
		if (!(tab[i] = ft_strsub(s, 0, !ft_isspace(*s) * word_len)))
			return (ft_strtab_free(tab));
		s += word_len;
		while (ft_isspace(*s))
			s++;
		i++;
	}
	tab[i] = NULL;
	return (tab);
}
