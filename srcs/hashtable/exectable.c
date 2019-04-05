#include <stdlib.h>
#include "libft.h"
#include "hashtable.h"
#include "exectable.h"

static void		del_execentry_val_fun(void *value, size_t value_size)
{
	(void)value_size;
	if (value != NULL)
	{
		free(((t_execentry*)value)->path);
		free(value);
	}
}

t_hashtable		*make_exectable(void)
{
	t_hashtable		*new_exectable;

	if ((new_exectable = make_def_hashtable()) == NULL)
		return (NULL);
	new_exectable->del_hashentry_val_fun = del_execentry_val_fun;
	return (new_exectable);
}

const char		*get_exec_path(t_hashtable *exectable, const char *name
		, int hit_incr)
{
	t_hashentry		*hashentry;

	hashentry = get_hashentry(exectable, name);
	if (hashentry != NULL)
	{
		((t_execentry*)hashentry->value)->hits += hit_incr;
		return (((t_execentry*)hashentry->value)->path);
	}
	return (NULL);
}

int				set_exec_path(t_hashtable *exectable, const char *name
		, const char *path, int def_hits)
{
	t_execentry		new_execentry;

	new_execentry.hits = def_hits;
	if ((new_execentry.path = ft_strdup(path)) == NULL)
		return (0);
	return (replace_hashentry(exectable, name, &new_execentry
				, sizeof(t_execentry)));
}
