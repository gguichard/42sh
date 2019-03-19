#ifndef ALIASTABLE_H
# define ALIASTABLE_H

# include "hashtable.h"

/*
** Retourne la valeur de l'alias, ou NULL si elle n'est pas dans la table.
*/
const char	*get_alias(t_hashtable *aliastable, const char *name);

/*
** Ajoute (ou remplace si deja existante) une entree dans l'aliastable liant
** l'alias name a la valeur alias_val si le nom est valide. Retourne 0 et set
** *error_msg a un message d'erreur en cas d'erreur, retourne 1 et set
** *error_msg a NULL en cas de succes.
** Si error_msg vaut NULL il ne sera pas set.
*/
int			set_alias_if_valid(t_hashtable *aliastable, const char *name
		, const char *alias_val, const char **error_msg);

#endif
