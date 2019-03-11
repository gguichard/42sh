/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit_shell.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/01 11:25:54 by tcollard          #+#    #+#             */
/*   Updated: 2019/03/02 16:02:26 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

static int			ft_word_counter(char *str, char c)
{
	int				i;
	int				nb_word;
	char			quote;

	i = 0;
	nb_word = (str[0] && str[0] != c) ? 1 : 0;
	while (str[i])
	{
		if (ft_isquote(str[i]) == 1)
		{
			quote = str[i];
			i += 1;
			while (str[i] && str[i] != quote)
				i += 1;
			i += 1;
		}
		if (i > 0 && str[i] == c && str[i + 1] != c)
			nb_word += 1;
		else if (str[i] == c && str[i + 1] == c)
			return (ft_error_splitshell());
		(str[i]) ? i += 1 : 0;
	}
	return (nb_word);
}

static int			ft_position_word(char *str, char c, int word_n)
{
	int		i;
	int		i_word;
	char	quote;

	i = 0;
	i_word = 0;
	(str[i] == c && str[i] != '\0') ? i += 1 : 0;
	while (str[i])
	{
		if (str[i] != c && i_word == word_n)
			return (i);
		if (ft_isquote(str[i]) == 1)
		{
			quote = str[i];
			i += 1;
			while (str[i] && str[i] != quote)
				i += 1;
			i += 1;
		}
		if (i > 0 && str[i] == c && str[i + 1] != c)
			i_word += 1;
		(str[i]) ? i += 1 : 0;
	}
	return (i);
}

static int			ft_counter_letter(char *str, char c, int word_n)
{
	int		pos_word;
	int		nb_letter;
	char	quote;

	nb_letter = 0;
	pos_word = ft_position_word(str, c, word_n);
	while (str[pos_word + nb_letter] && str[pos_word + nb_letter] != c)
	{
		if (ft_isquote(str[pos_word + nb_letter]) == 1)
		{
			quote = str[pos_word + nb_letter];
			nb_letter += 1;
			while (str[pos_word + nb_letter] && str[pos_word + nb_letter]
				!= quote)
				nb_letter += 1;
			nb_letter += 1;
		}
		else
			nb_letter += 1;
	}
	return (nb_letter);
}

static void			ft_fill(char *str, char c, int word_n, char **split)
{
	int		i;
	int		j;
	int		nb_letter;

	j = 0;
	nb_letter = ft_counter_letter(str, c, word_n);
	if (!(split[word_n] = (char*)malloc(sizeof(char) * (nb_letter + 1))))
		ft_exit_malloc();
	i = ft_position_word(str, c, word_n);
	while (str[i] && j < nb_letter)
		split[word_n][j++] = str[i++];
	split[word_n][j] = '\0';
}

char				**ft_strsplit_shell(char *str, char c)
{
	int				nb_word;
	int				i;
	char			**split;

	i = 0;
	if (!str)
		return (NULL);
	if ((nb_word = ft_word_counter(str, c)) == -1)
		return (NULL);
	if (!(split = (char**)malloc(sizeof(char*) * (nb_word + 1))))
		ft_exit_malloc();
	while (i < nb_word)
	{
		ft_fill(str, c, i, split);
		i += 1;
	}
	split[nb_word] = 0;
	return (split);
}
