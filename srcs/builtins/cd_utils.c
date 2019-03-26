/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/24 00:24:00 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/29 00:03:53 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "builtins.h"

static size_t	count_slash(char *dir)
{
	int	size;

	size = 0;
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

	if ((size = count_slash(dir)) == 0
			|| (comps = (char **)malloc((size + 1) * sizeof(char *))) == NULL)
		return (NULL);
	index = 0;
	while (index < size)
	{
		dir = ft_strchr(dir, '/');
		if (dir == NULL)
		{
			free(comps);
			return (NULL);
		}
		while (*dir == '/')
			dir++;
		*(dir - 1) = '\0';
		comps[index] = dir;
		index++;
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
	*dir = '/';
	while (comps[index] != NULL && comps[index][0] != '\0')
	{
		dir += 1;
		comp_len = ft_strlen(comps[index]);
		ft_memcpy(dir, comps[index], comp_len);
		dir += comp_len;
		*dir = '/';
		index++;
	}
	*dir = '\0';
}

void			set_dir_to_canonical_form(char *dir)
{
	char	**comps;
	size_t	index;
	int		ret;

	if ((comps = dir_split_comps(dir)) == NULL)
		return ;
	index = 0;
	while (comps[index] != NULL)
	{
		ret = 0;
		if (ft_strequ(".", comps[index]) && (ret = 1) == 1)
			dir_delete_comp(comps, index);
		else if (ft_strequ("..", comps[index]) && (ret = 1) == 1)
		{
			dir_delete_comp(comps, index);
			if (index > 0)
				dir_delete_comp(comps, --index);
		}
		if (!ret)
			index++;
	}
	dir_glue_comps(comps, dir);
	free(comps);
	if (dir[0] == '\0')
		ft_strcpy(dir, "/");
}
