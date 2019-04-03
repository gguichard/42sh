/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_path.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 19:33:19 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/02 19:36:06 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "vars.h"
#include "check_path.h"
#include "builtins.h"

char	*search_in_cd_path(t_alloc *alloc, const char *path, const char *pwd)
{
	t_var	*var;
	char	*tmp;
	char	*cd_path;
	char	*next_sep;
	char	*full_path;

	if ((var = get_var(alloc->vars, "CDPATH")) == NULL
			|| var->value == NULL || (tmp = ft_strdup(var->value)) == NULL)
		return (NULL);
	cd_path = tmp;
	full_path = NULL;
	while (1)
	{
		if ((next_sep = ft_strchr(cd_path, ':')) != NULL)
			*next_sep = '\0';
		full_path = join_path((ft_strequ(cd_path, ".") ? pwd : cd_path), path);
		if (full_path && check_dir_rights(full_path, X_OK) == ERRC_NOERROR)
			break ;
		ft_strdel(&full_path);
		if (next_sep == NULL)
			break ;
		cd_path = next_sep + 1;
	}
	free(tmp);
	return (full_path);
}
