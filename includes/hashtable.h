/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 14:01:51 by fwerner           #+#    #+#             */
/*   Updated: 2019/04/08 14:01:53 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASHTABLE_H
# define HASHTABLE_H

# include "libft.h"

# define DEF_BUCKET_COUNT 2053

typedef struct	s_hashentry
{
	char		*key;
	void		*value;
	size_t		value_size;
}				t_hashentry;

typedef struct	s_hashtable
{
	t_list		**buckets;
	size_t		bucket_count;
	size_t		(*hash_fun)(const char *str);
	void		(*del_hashentry_val_fun)(void *value, size_t value_size);
}				t_hashtable;

/*
** Fonction de suppression de valeur de hashentry par defaut qui se contente
** de free value.
*/
void			def_del_hashentry_val_fun(void *value, size_t value_size);

/*
** Fonction de hashage d'une string par defaut.
*/
size_t			def_hash_fun(const char *str);

/*
** Retourne une hashtable initilialisee avec les parametres par defaut, ou NULL
** si erreur.
*/
t_hashtable		*make_def_hashtable(void);

/*
** Retourne le hashentry ayant pour clef key, ou NULL s'il n'existe pas.
*/
t_hashentry		*get_hashentry(t_hashtable *hashtable, const char *key);

/*
** Set l'entree de la hashtable ayant pour clef key a la valeur value. Si
** l'entree existe deja ne fait rien d'autre que retourner 1. La valeur et la
** clef sont ajoutes via une shallow copie. Retourne 0 en cas d'erreur et 1
** en cas de succes.
*/
int				add_hashentry(t_hashtable *hashtable, const char *key
		, const void *value, size_t value_size);

/*
** Set l'entree de la hashtable ayant pour clef key a la valeur value. Si
** l'entree existe deja elle sera remplacee, sinon elle est cree. La valeur
** et la clef sont ajoutes via une shallow copie. Retourne 0 en cas d'erreur
** et 1 en cas de succes.
*/
int				replace_hashentry(t_hashtable *hashtable, const char *key
		, const void *value, size_t value_size);

/*
** Supprime et free l'element ayant pour clef key de la hashtable. Retourne 1
** si l'element existait et la suppression a bien eu lieu, 0 sinon.
*/
int				remove_hashentry(t_hashtable *hashtable, const char *key);

/*
** Free l'integralite des elements de la hashtable ainsi que la hashtable
** elle-meme.
*/
void			delete_hashtable(t_hashtable *hashtable);

/*
** Free le contenu de la hashentry ainsi que la hashentry elle-meme (le tout si
** elle est != NULL). Retourne toujours 0.
*/
int				delete_hashentry(t_hashtable *hashtable, t_hashentry *entry);

/*
** Free toutes les hashentry de la hashtable. Retourne toujours 0.
*/
int				delete_hashentries(t_hashtable *hashtable);

#endif
