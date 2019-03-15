#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "hashtable.h"
#include "aliastable.h"

const char	*get_alias(t_hashtable *aliastable, const char *name)
{
	t_hashentry		*hashentry;

	hashentry = get_hashentry(aliastable, name);
	if (hashentry != NULL)
		return ((const char*)hashentry->value);
	return (NULL);
}

int			set_alias_if_valid_and_print_err(t_hashtable *aliastable
		, const char *name, const char *alias_val)
{
	size_t	idx;

	idx = 0;
	while (name[idx] != '\0')
	{
		if (!ft_isalnum(name[idx]) && ft_strchr("_!%,@", name[idx]) == NULL)
		{
			ft_dprintf(STDERR_FILENO, "42sh: alias: %s: invalid alias name\n"
					, name);
			return (0);
		}
		++idx;
	}
	if (!replace_hashentry(aliastable, name, alias_val, sizeof(const char*)))
	{
		ft_dprintf(STDERR_FILENO, "42sh: alias: unknown error\n");
		return (0);
	}
	return (1);
}
