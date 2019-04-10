/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   source.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 10:22:28 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/10 21:32:49 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "error.h"
#include "check_path.h"
#include "execution.h"

static char	*search_path_for_source(t_alloc *alloc, const char *file_name
		, t_error *error)
{
	char	*path;

	path = NULL;
	*error = ERRC_FILENOTFOUND;
	if (ft_strchr(file_name, '/') == NULL)
		path = search_in_path(alloc->vars, file_name, R_OK, error);
	if (*error == ERRC_FILENOTFOUND)
	{
		*error = check_file_rights(file_name, FT_FILE, R_OK);
		if (*error == ERRC_NOERROR && (path = ft_strdup(file_name)) == NULL)
			*error = ERRC_UNEXPECTED;
	}
	return (path);
}

int			builtin_source(t_ast *elem, t_alloc *alloc)
{
	t_error	error;
	char	*path;

	if (elem->input[1] == NULL)
	{
		ft_dprintf(STDERR_FILENO, "42sh: %s: filename argument required\n"
				"%s: usage: %s filename\n"
				, elem->input[0], elem->input[0], elem->input[0]);
		return (2);
	}
	path = search_path_for_source(alloc, elem->input[1], &error);
	if (path != NULL && error == ERRC_NOERROR)
		error = source_file(alloc, path);
	free(path);
	if (error != ERRC_NOERROR)
	{
		ft_dprintf(STDERR_FILENO, "42sh: %s: %s: %s\n", elem->input[0]
				, elem->input[1], error_to_str(error));
		return (1);
	}
	return (0);
}
