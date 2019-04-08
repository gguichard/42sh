/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:36:18 by fwerner           #+#    #+#             */
/*   Updated: 2019/04/08 13:36:20 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "hashtable.h"

/*
** Retourne le hashentry ayant pour clef key et pour hash hash, ou NULL
** s'il n'existe pas. Cette fonction a pour but d'eviter de recalculer le hash.
*/

static t_hashentry	*get_hashentry_with_hash(t_hashtable *hashtable
		, const char *key, size_t hash)
{
	t_list	*bucket;

	bucket = hashtable->buckets[hash % hashtable->bucket_count];
	while (bucket != NULL)
	{
		if (ft_strequ(((t_hashentry*)bucket->content)->key, key))
			return ((t_hashentry*)bucket->content);
		bucket = bucket->next;
	}
	return (NULL);
}

t_hashentry			*get_hashentry(t_hashtable *hashtable, const char *key)
{
	return (get_hashentry_with_hash(hashtable, key, hashtable->hash_fun(key)));
}

/*
** Malloc et retourne un nouveau hashentry avec une copie de key et de value.
** Retourne NULL en cas d'erreur.
*/

static t_hashentry	*make_hashentry(const char *key, const void *value
		, size_t value_size)
{
	t_hashentry		*new_entry;

	if ((new_entry = (t_hashentry*)malloc(sizeof(t_hashentry))) == NULL)
		return (NULL);
	if ((new_entry->key = ft_strdup(key)) == NULL)
	{
		free(new_entry);
		return (NULL);
	}
	if (value != NULL)
	{
		if ((new_entry->value = malloc(value_size)) == NULL)
		{
			free(new_entry->key);
			free(new_entry);
			return (NULL);
		}
		ft_memcpy(new_entry->value, value, value_size);
	}
	else
		new_entry->value = NULL;
	return (new_entry);
}

int					add_hashentry(t_hashtable *hashtable, const char *key
		, const void *value, size_t value_size)
{
	size_t			hash;
	t_hashentry		*entry;
	t_list			*new_bucket_el;

	hash = hashtable->hash_fun(key);
	entry = get_hashentry_with_hash(hashtable, key, hash);
	if (entry == NULL)
	{
		if ((entry = make_hashentry(key, value, value_size)) == NULL
				|| (new_bucket_el = ft_lstnew(entry
						, sizeof(t_hashentry))) == NULL)
			return (delete_hashentry(hashtable, entry));
		ft_lstadd(hashtable->buckets + (hash % hashtable->bucket_count)
				, new_bucket_el);
		free(entry);
	}
	return (1);
}

int					replace_hashentry(t_hashtable *hashtable, const char *key
		, const void *value, size_t value_size)
{
	size_t			hash;
	t_hashentry		*entry;
	void			*new_value;
	t_list			*nw_el;

	hash = hashtable->hash_fun(key);
	entry = get_hashentry_with_hash(hashtable, key, hash);
	if (entry != NULL)
	{
		if (value != NULL && (new_value = malloc(value_size)) == NULL)
			return (0);
		hashtable->del_hashentry_val_fun(entry->value, entry->value_size);
		if (value != NULL)
			ft_memcpy(new_value, value, value_size);
		entry->value = (value == NULL ? NULL : new_value);
	}
	else if (entry == NULL)
	{
		if ((entry = make_hashentry(key, value, value_size)) == NULL
				|| (nw_el = ft_lstnew(entry, sizeof(t_hashentry))) == NULL)
			return (delete_hashentry(hashtable, entry));
		ft_lstadd(hashtable->buckets + (hash % hashtable->bucket_count), nw_el);
		free(entry);
	}
	return (1);
}
