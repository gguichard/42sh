/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_path.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 15:23:54 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/12 11:30:57 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "exectable.h"
#include "check_path.h"

static char			*search_exec(t_list *vars, const char *name, int *hashable
		, t_error *err)
{
	char	*path;

	path = search_in_path(vars, name, X_OK, err);
	if (*err != ERRC_NOERROR)
	{
		ft_strdel(&path);
		return (NULL);
	}
	*hashable = 1;
	return (path);
}

static const char	*check_right_alias(const char *alias, t_error *err)
{
	*err = check_file_rights(alias, FT_FILE, X_OK);
	if (*err != ERRC_NOERROR)
		return (NULL);
	return (alias);
}

char				*exec_path(t_alloc *alloc, const char *name, int *hashable
		, t_error *err)
{
	const char	*alias;
	char		*path_exec;

	*err = ERRC_NOERROR;
	path_exec = NULL;
	alias = NULL;
	*hashable = 0;
	if (!ft_strchr(name, '/'))
	{
		if ((alias = get_exec_path(alloc->exectable, name, 1)) == NULL)
			path_exec = search_exec(alloc->vars, name, hashable, err);
		else if ((alias = check_right_alias(alias, err)) != NULL)
			path_exec = ft_strdup(alias);
	}
	else
	{
		if ((*err = check_file_rights(name, FT_FILE, X_OK)) != ERRC_NOERROR)
			return (NULL);
		else
			path_exec = ft_strdup(name);
	}
	if (path_exec == NULL && *err == ERRC_NOERROR)
		*err = ERRC_UNEXPECTED;
	return (path_exec);
}
