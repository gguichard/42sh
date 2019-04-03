#include "shell.h"
#include "expand.h"

static unsigned int	ft_word_counter(char const *s)
{
	int				i;
	unsigned int	nb_word;

	nb_word = 0;
	i = 0;
	if (s[0] && ft_isspace(s[0]) == 0)
		nb_word++;
	while (s[i])
	{
		if (i > 0 && ft_isspace(s[i]) == 0 && ft_isspace(s[i - 1]))
			nb_word++;
		i++;
	}
	return (nb_word);
}

static int			ft_position_word(char const *s, unsigned int word_n)
{
	int				i;
	unsigned int	i_word;

	i = 0;
	i_word = 0;
	while (s[i] && ft_isspace(s[i]))
		i++;
	while (s[i])
	{
		if (ft_isspace(s[i]) == 0 && i_word == word_n)
			return (i);
		if (i > 0 && ft_isspace(s[i]) && ft_isspace(s[i + 1]) == 0)
			i_word++;
		i++;
	}
	return (i);
}

static int			ft_counter_letter(char const *s, unsigned int word_n)
{
	int	pos_word;
	int	nb_letter;

	nb_letter = 0;
	pos_word = ft_position_word(s, word_n);
	while (s[pos_word] && ft_isspace(s[pos_word]) == 0)
	{
		pos_word++;
		nb_letter++;
	}
	return (nb_letter);
}

static void			ft_fill(char const *s, unsigned int wrd_n, char **split)
{
	int	i;
	int	j;

	i = ft_position_word(s, wrd_n);
	j = 0;
	while (s[i] && ft_isspace(s[i]) == 0)
	{
		split[wrd_n][j] = s[i];
		i++;
		j++;
	}
	split[wrd_n][j] = '\0';
}

char				**ft_splitwhitespace(char const *s)
{
	unsigned int	nb_word;
	unsigned int	i;
	char			**split;
	int				nb_letter;

	i = 0;
	if (!s)
		return (0);
	nb_word = ft_word_counter(s);
	if (!(split = (char**)malloc(sizeof(char*) * (nb_word + 1))))
		return (NULL);
	while (i < nb_word)
	{
		nb_letter = ft_counter_letter(s, i);
		if (!(split[i] = (char*)malloc(sizeof(char) * (nb_letter + 1))))
			return (NULL);
		ft_fill((char const *)s, i, split);
		i++;
	}
	split[nb_word] = 0;
	return (split);
}
