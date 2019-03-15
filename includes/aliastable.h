#ifndef ALIASTABLE_H
# define ALIASTABLE_H

# include "hashtable.h"

/*
** Retourne la valeur de l'alias, ou NULL si elle n'est pas dans la table.
*/
const char	*get_alias(t_hashtable *aliastable, const char *name);

/*
** Ajoute (ou remplace si deja existante) une entree dans l'aliastable liant
** l'alias name a la valeur alias_val. Retourne 0 en cas d'erreur et 1 en
** cas de succes.
*/
int			set_alias(t_hashtable *aliastable, const char *name
		, const char *alias_val);

#endif
