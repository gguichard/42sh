/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 15:26:54 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/28 15:09:59 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

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

/*
** Free la variable var et retourne err.
** Fonction utilitaire ayant pour but de reduire le nombre de ligne.
*/
t_error		del_then_ret_err(void *var, t_error err);

const char	*error_to_str(t_error error);

#endif
