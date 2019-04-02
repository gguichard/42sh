/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 00:24:00 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/02 19:23:15 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "builtins.h"

static size_t	count_components(char *dir)
{
	int	size;

	size = 1;
	while (*dir != '\0')
	{
		if (*dir != '/')
			dir++;
		else
		{
			size++;
			while (*dir == '/')
				dir++;
		}
	}
	return (size);
}

static char		**dir_split_comps(char *dir)
{
	size_t	size;
	size_t	index;
	char	**comps;
	char	*slash;

	if ((size = count_components(dir)) == 0)
		return (NULL);
	else if ((comps = (char **)malloc((size + 1) * sizeof(char *))) == NULL)
		return (NULL);
	index = 0;
	while (1)
	{
		slash = ft_strchr(dir, '/');
		if (slash != NULL)
			*slash = '\0';
		comps[index] = (dir == slash) ? "/" : dir;
		index++;
		if (slash == NULL)
			break ;
		dir = slash + 1;
		while (dir[0] == '/')
			dir++;
	}
	comps[index] = NULL;
	return (comps);
}

static void		dir_delete_comp(char **comps, size_t index)
{
	while (comps[index] != NULL)
	{
		comps[index] = comps[index + 1];
		index++;
	}
}

static void		dir_glue_comps(char **comps, char *dir)
{
	size_t	index;
	size_t	comp_len;

	index = 0;
	while (comps[index] != NULL)
	{
		if (index > 0 && !ft_strequ(comps[index - 1], "/"))
		{
			*dir = '/';
			dir++;
		}
		comp_len = ft_strlen(comps[index]);
		ft_memcpy(dir, comps[index], comp_len);
		dir += comp_len;
		index++;
	}
	*dir = '\0';
}

void			set_dir_to_canonical_form(char *dir)
{
	char	**comps;
	size_t	index;

	if ((comps = dir_split_comps(dir)) == NULL)
		return ;
	index = 0;
	while (comps[index] != NULL)
	{
		if (ft_strequ(".", comps[index]))
			dir_delete_comp(comps, index);
		else if (ft_strequ("..", comps[index]))
		{
			dir_delete_comp(comps, index);
			if (index > 0)
				dir_delete_comp(comps, --index);
		}
		else
			index++;
	}
	dir_glue_comps(comps, dir);
	free(comps);
}
