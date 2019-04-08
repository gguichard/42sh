#include <stdlib.h>
#include "libft.h"
#include "expand.h"

static size_t	ft_word_len(char const *s)
{
	size_t	len;

	len = 0;
	while (s[len] != '\0' && !ft_isspace(s[len]))
		len++;
	return (len);
}

static size_t	ft_count_words(char const *s)
{
	size_t	words;

	words = 0;
	while (*s != '\0')
	{
		if (ft_isspace(*s))
			s++;
		else
		{
			words++;
			s += ft_word_len(s);
		}
	}
	return (words);
}

char			**ft_splitwhitespace(const char *s)
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
	tab[words] = 0;
	i = 0;
	while (i < words)
	{
		while (ft_isspace(*s))
			s++;
		word_len = ft_word_len(s);
		if (!(tab[i] = ft_strsub(s, 0, word_len)))
			return (ft_strtab_free(tab));
		s += word_len;
		i++;
	}
	return (tab);
}
