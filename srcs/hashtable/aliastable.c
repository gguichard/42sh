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

int			set_alias_if_valid(t_hashtable *aliastable, const char *name
		, const char *alias_val, const char **error_msg)
{
	size_t	idx;

	if (error_msg != NULL)
		*error_msg = NULL;
	idx = 0;
	while (name[idx] != '\0')
	{
		if (!ft_isalnum(name[idx]) && ft_strchr("_!%,@", name[idx]) == NULL)
		{
			if (error_msg != NULL)
				*error_msg = "invalid alias name";
			return (0);
		}
		++idx;
	}
	if (!replace_hashentry(aliastable, name, alias_val
				, ft_strlen(alias_val) + 1))
	{
		if (error_msg != NULL)
			*error_msg = "unknown error";
		return (0);
	}
	return (1);
}
