#ifndef EXPAND_VARS_H
# define EXPAND_VARS_H

# include "shell.h"

/*
** Alloue et retourne une nouvelle string avec le home expand si present.
** Retourne NULL en cas d'erreur.
*/
char				*expand_home(const char *str, t_var *var_lst
		, int remove_home_ending_slash);

#endif
