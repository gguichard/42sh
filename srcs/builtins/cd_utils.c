/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 00:24:00 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/03 16:01:52 by gguichar         ###   ########.fr       */
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
	size_t	idx;
	char	**comps;
	char	*slash;

	if ((size = count_components(dir)) == 0)
		return (NULL);
	else if ((comps = (char **)malloc((size + 1) * sizeof(char *))) == NULL)
		return (NULL);
	idx = 0;
	while (*dir != '\0')
	{
		slash = ft_strchr(dir, '/');
		if (slash != NULL)
			*slash = '\0';
		comps[idx] = (dir == slash) ? "/" : dir;
		idx++;
		if (slash == NULL)
			break ;
		dir = slash + 1;
		while (dir[0] == '/')
			dir++;
	}
	comps[idx] = NULL;
	return (comps);
}

static void		dir_delete_comp(char **comps, size_t idx)
{
	while (comps[idx] != NULL)
	{
		comps[idx] = comps[idx + 1];
		idx++;
	}
}

static void		dir_glue_comps(char **comps, char *dir)
{
	size_t	idx;
	size_t	offset;
	size_t	comp_len;

	idx = 0;
	offset = 0;
	while (comps[idx] != NULL)
	{
		if (offset > 0 && dir[offset - 1] != '/')
		{
			dir[offset] = '/';
			offset++;
		}
		comp_len = ft_strlen(comps[idx]);
		ft_memcpy(dir + offset, comps[idx], comp_len);
		offset += comp_len;
		idx++;
	}
	dir[offset] = '\0';
}

void			set_dir_to_canonical_form(char *dir)
{
	char	**comps;
	size_t	idx;

	if ((comps = dir_split_comps(dir)) == NULL)
		return ;
	idx = 0;
	while (comps[idx] != NULL)
	{
		if (ft_strequ(".", comps[idx]))
			dir_delete_comp(comps, idx);
		else if (ft_strequ("..", comps[idx]))
		{
			dir_delete_comp(comps, idx);
			if (idx > 0)
				dir_delete_comp(comps, --idx);
		}
		else
			idx++;
	}
	dir_glue_comps(comps, dir);
	free(comps);
	if (dir[0] == '\0')
		ft_strcpy(dir, "/");
}
