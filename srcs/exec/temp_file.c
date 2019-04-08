/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   temp_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/07 18:57:10 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/08 09:58:16 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "libft.h"
#include "shell.h"
#include "error.h"
#include "check_path.h"
#include "vars.h"

static const char	*get_temp_dir(t_list *vars)
{
	static const char	*paths[] = {"/tmp", "/var/tmp", "/usr/tmp"};
	const char			*temp_dir;
	size_t				idx;

	temp_dir = get_var_value_or_null(vars, "TMPDIR");
	if (temp_dir == NULL || check_dir_rights(temp_dir, W_OK) != ERRC_NOERROR)
	{
		temp_dir = NULL;
		idx = 0;
		while (temp_dir == NULL && idx < 3)
		{
			if (check_dir_rights(paths[idx], W_OK) == ERRC_NOERROR)
				temp_dir = paths[idx];
			idx++;
		}
	}
	return (temp_dir);
}

static int			constuct_tempfile_path(char *buffer, const char *temp_dir
		, const char *file_prefix, unsigned long file_num)
{
	char	*file_id;
	size_t	temp_dir_len;
	size_t	file_prefix_len;
	size_t	file_id_len;

	file_id = ft_ulltoa(file_num);
	if (file_id == NULL)
		return (0);
	temp_dir_len = ft_strlen(temp_dir);
	file_prefix_len = ft_strlen(file_prefix);
	file_id_len = ft_strlen(file_id);
	if ((temp_dir_len + file_id_len + file_prefix_len + 1) > PATH_MAX)
	{
		free(file_id);
		return (0);
	}
	ft_memcpy(buffer, temp_dir, temp_dir_len);
	buffer[temp_dir_len] = '/';
	ft_memcpy(buffer + temp_dir_len + 1, file_prefix, file_prefix_len);
	ft_memcpy(buffer + temp_dir_len + file_prefix_len + 1, file_id
			, file_id_len + 1);
	free(file_id);
	return (1);
}

char				*get_tempfile_path(t_alloc *alloc, const char *file_prefix)
{
	static unsigned long	file_num = 1L;
	static size_t			ntemp_files = 0;
	const char				*temp_dir;
	char					*tempfile_path;
	int						fd;

	if ((temp_dir = get_temp_dir(alloc->vars)) == NULL)
		return (NULL);
	tempfile_path = (char *)malloc((PATH_MAX + 1) * sizeof(char));
	if (tempfile_path == NULL)
		return (NULL);
	while (1)
	{
		file_num = (file_num << 1)
			^ (unsigned long)alloc->ppid
			^ (unsigned long)ntemp_files++;
		if (!constuct_tempfile_path(tempfile_path, temp_dir, file_prefix
					, file_num))
			return (ft_memdel((void **)&tempfile_path));
		fd = open(tempfile_path, O_RDONLY);
		if (fd == -1)
			break ;
		close(fd);
	}
	return (tempfile_path);
}
