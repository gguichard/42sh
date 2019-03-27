#ifndef EXECUTE_H
# define EXECUTE_H

# include "libft.h"
# include "shell.h"
# include "error.h"

/*
** Alloue et retourne le chemin de l'executable exec_name s'il est present dans
** le PATH, sinon retourne NULL. Set dans tous les cas error avec l'erreur
** rencontree (ERRC_NOERROR s'il n'y a pas eu d'erreur). Un free du retour
** est necessaire.
*/
char			*search_exec(t_list *vats, const char *exec_name
		, t_error *error);

#endif
