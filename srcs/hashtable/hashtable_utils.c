#include <stdlib.h>
#include "libft.h"
#include "hashtable.h"

void			def_del_hashentry_val_fun(void *value, size_t value_size)
{
	(void)value_size;
	free(value);
}

size_t			def_hash_fun(const char *str)
{
	size_t			hash;
	unsigned int	c;

	hash = 5381;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;
	return (hash);
}

t_hashtable		*make_def_hashtable(void)
{
	t_hashtable		*new_hashtable;

	if ((new_hashtable = (t_hashtable*)(malloc(sizeof(t_hashtable)))) == NULL)
		return (NULL);
	new_hashtable->bucket_count = DEF_BUCKET_COUNT;
	new_hashtable->buckets = (t_list**)ft_memalloc(sizeof(t_list*)
			* new_hashtable->bucket_count);
	if (new_hashtable->buckets == NULL)
	{
		free(new_hashtable);
		return (NULL);
	}
	new_hashtable->hash_fun = def_hash_fun;
	new_hashtable->del_hashentry_val_fun = def_del_hashentry_val_fun;
	return (new_hashtable);
}
