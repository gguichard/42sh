/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ac_rdir_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 11:49:55 by fwerner           #+#    #+#             */
/*   Updated: 2019/04/08 15:00:26 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include "shell.h"
#include "autocomplete.h"

int		valid_file_for_ac(t_ac_rdir_inf *acrd)
{
	if (acrd->cur_file_name[0] != '.' || acrd->file_word[0] == '.')
	{
		if (acrd->need_to_be_cmd)
		{
			if (!acrd->force_exec_type && !(S_ISREG(acrd->stat_buf.st_mode)
						&& access(acrd->cur_file_path, X_OK) == 0)
					&& (acrd->dir_type == DTYPE_NOT_A_DIR
						|| !S_ISDIR(acrd->stat_buf.st_mode)))
				return (0);
		}
		if (acrd->dir_type == DTYPE_IS_A_DIR)
		{
			if (!S_ISDIR(acrd->stat_buf.st_mode))
				return (0);
		}
		if (acrd->dir_type == DTYPE_NOT_A_DIR)
		{
			if (S_ISDIR(acrd->stat_buf.st_mode))
				return (0);
		}
		if (ft_strnequ(acrd->cur_file_name, acrd->file_word
					, acrd->file_word_len))
			return (1);
	}
	return (0);
}

t_list	*make_new_choice(t_ac_rdir_inf *acrd)
{
	char	*tmp_file_name;
	size_t	tmp_file_name_len;
	t_list	*new_elem;

	tmp_file_name_len = ft_strlen(acrd->cur_file_name);
	if ((tmp_file_name = (char*)malloc(sizeof(char) * (1 + tmp_file_name_len
					+ (!acrd->force_exec_type
						&& S_ISDIR(acrd->stat_buf.st_mode) ? 1 : 0)))) == NULL)
		return (NULL);
	ft_memcpy(tmp_file_name, acrd->cur_file_name, tmp_file_name_len + 1);
	if (!acrd->force_exec_type && S_ISDIR(acrd->stat_buf.st_mode))
	{
		ft_memcpy(tmp_file_name + tmp_file_name_len, "/", 2);
		++tmp_file_name_len;
	}
	new_elem = ft_lstnew(tmp_file_name, tmp_file_name_len + 1);
	free(tmp_file_name);
	return (new_elem);
}

int		readdir_to_dirent(t_ac_rdir_inf *acrd, t_ac_suff_inf *acs)
{
	struct dirent	*tmp_dirent;

	while ((tmp_dirent = readdir(acrd->dir)) != NULL)
	{
		acrd->cur_file_name = tmp_dirent->d_name;
		if ((acrd->cur_file_path = ft_strjoin(acrd->dir_to_use
						, acrd->cur_file_name)) == NULL)
		{
			acs->suff = NULL;
			return (0);
		}
		if (stat(acrd->cur_file_path, &(acrd->stat_buf)) == -1)
		{
			if (acrd->need_to_be_cmd
					|| lstat(acrd->cur_file_path, &(acrd->stat_buf)) == -1)
			{
				ft_memdel((void**)&(acrd->cur_file_path));
				continue ;
			}
		}
		return (1);
	}
	return (0);
}

int		init_ac_rdir(const char *word, t_ac_rdir_inf *acrd
		, int need_to_be_cmd, t_dir_type dir_type)
{
	char	*last_slash;

	acrd->need_to_be_cmd = need_to_be_cmd;
	acrd->force_exec_type = 0;
	acrd->dir_type = dir_type;
	if ((last_slash = ft_strrchr(word, '/')) == NULL)
		acrd->dir_to_use = ft_strdup("./");
	else
		acrd->dir_to_use = ft_strndup(word, last_slash - word + 1);
	if (acrd->dir_to_use == NULL || (acrd->file_word = ft_strdup(last_slash
					== NULL ? word : last_slash + 1)) == NULL)
	{
		ft_memdel((void**)&(acrd->dir_to_use));
		return (0);
	}
	acrd->file_word_len = ft_strlen(acrd->file_word);
	acrd->dir = NULL;
	acrd->cur_file_path = NULL;
	acrd->cur_file_name = NULL;
	return (1);
}

void	delete_ac_rdir(t_ac_rdir_inf *acrd)
{
	if (acrd->dir != NULL)
	{
		closedir(acrd->dir);
		acrd->dir = NULL;
	}
	ft_memdel((void**)&(acrd->dir_to_use));
	ft_memdel((void**)&(acrd->file_word));
	ft_memdel((void**)&(acrd->cur_file_path));
}
