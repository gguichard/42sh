#include <stdlib.h>
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

int			set_alias(t_hashtable *aliastable, const char *name
		, const char *alias_val)
{
	return (replace_hashentry(aliastable, name, alias_val
				, sizeof(const char*)));
}
