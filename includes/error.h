/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <fwerner@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 14:02:46 by fwerner           #+#    #+#             */
/*   Updated: 2019/04/08 14:31:46 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void			exec_file_error(t_error err, const char *file);

/*
** Free la variable var et retourne err.
** Fonction utilitaire ayant pour but de reduire le nombre de ligne.
*/
t_error			del_then_ret_err(void *var, t_error err);

const char		*error_to_str(t_error error);

#endif
