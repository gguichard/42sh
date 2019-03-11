/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/03 16:04:31 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/22 12:35:46 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "error.h"

t_error		del_then_ret_err(void *var, t_error err)
{
	free(var);
	return (err);
}

const char	*error_to_str(t_error error)
{
	if (error == ERRC_FILENOTFOUND)
		return ("No such file or directory");
	else if (error == ERRC_CMDNOTFOUND)
		return ("Command not found");
	else if (error == ERRC_NONEEDEDRIGHTS)
		return ("Permission denied");
	else if (error == ERRC_ISADIR)
		return ("Is a directory");
	else if (error == ERRC_NOTADIR)
		return ("Not a directory");
	else
		return ("Unknown error");
}
