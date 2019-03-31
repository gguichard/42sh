#ifndef ERROR_H
# define ERROR_H

# include "shell.h"

typedef enum	e_error
{
	ERRC_NOERROR = 0,
	ERRC_UNEXPECTED = 1,
	ERRC_FILENOTFOUND,
	ERRC_CMDNOTFOUND,
	ERRC_NOTADIR,
	ERRC_ISADIR,
	ERRC_NONEEDEDRIGHTS,
	ERRC_LEXERROR,
	ERRC_INCOMPLETECMD
}				t_error;

void			ft_exit_malloc(void);
void			exec_file_error(t_error err, const char *file);

/*
** Free la variable var et retourne err.
** Fonction utilitaire ayant pour but de reduire le nombre de ligne.
*/
t_error			del_then_ret_err(void *var, t_error err);

const char		*error_to_str(t_error error);

#endif
