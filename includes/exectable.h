/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exectable.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 14:02:34 by fwerner           #+#    #+#             */
/*   Updated: 2019/04/08 14:02:35 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECTABLE_H
# define EXECTABLE_H

# include "hashtable.h"

typedef struct	s_execentry
{
	int			hits;
	char		*path;
}				t_execentry;

/*
** Construit une table de hash pour les executables.
*/
t_hashtable		*make_exectable(void);

/*
** Retourne le chemin de l'executable, ou NULL s'il n'est pas dans la table.
** Incremente le compteur hits de hit_incr si l'entree existe.
*/
const char		*get_exec_path(t_hashtable *exectable, const char *name
		, int hit_incr);

/*
** Ajoute (ou remplace si deja existante) une entree dans l'exectable liant
** l'executable name au chemin path, hits sera set a def_hits. Retourne 0 en
** cas d'erreur et 1 en cas de succes.
*/
int				set_exec_path(t_hashtable *exectable, const char *name
		, const char *path, int def_hits);

#endif
