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

int		remove_hashentry(t_hashtable *hashtable, const char *key)
{
	int				bucket_idx;
	t_list			*old_elem;
	t_list			*cur_elem;
	t_hashentry		*cur_entry;

	bucket_idx = hashtable->hash_fun(key) % hashtable->bucket_count;
	old_elem = NULL;
	cur_elem = hashtable->buckets[bucket_idx];
	while (cur_elem != NULL)
	{
		cur_entry = (t_hashentry*)cur_elem->content;
		if (ft_strequ(key, cur_entry->key))
		{
			delete_hashentry(hashtable, cur_entry);
			if (old_elem == NULL)
				hashtable->buckets[bucket_idx] = cur_elem->next;
			else
				old_elem->next = cur_elem->next;
			free(cur_elem);
			break;
		}
		old_elem = cur_elem;
		cur_elem = cur_elem->next;
	}
	return (cur_elem != NULL);
}
