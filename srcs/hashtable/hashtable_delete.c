#include <stdlib.h>
#include "libft.h"
#include "hashtable.h"

int		delete_hashentry(t_hashtable *hashtable, t_hashentry *entry)
{
	if (entry != NULL)
	{
		free(entry->key);
		hashtable->del_hashentry_val_fun(entry->value, entry->value_size);
		free(entry);
	}
	return (0);
}

int		delete_hashentries(t_hashtable *hashtable)
{
	size_t	bucket_idx;
	t_list	*cur_bucket;
	t_list	*old_bucket;

	bucket_idx = 0;
	while (bucket_idx < hashtable->bucket_count)
	{
		cur_bucket = hashtable->buckets[bucket_idx];
		while (cur_bucket != NULL)
		{
			delete_hashentry(hashtable, (t_hashentry*)cur_bucket->content);
			old_bucket = cur_bucket;
			cur_bucket = cur_bucket->next;
			free(old_bucket);
		}
		hashtable->buckets[bucket_idx] = NULL;
		++bucket_idx;
	}
	return (0);
}

void	delete_hashtable(t_hashtable *hashtable)
{
	delete_hashentries(hashtable);
	free(hashtable->buckets);
	free(hashtable);
}
