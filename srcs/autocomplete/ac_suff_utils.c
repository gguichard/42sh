#include <stdlib.h>
#include <sys/stat.h>
#include "shell.h"
#include "autocomplete.h"

int		build_ac_suff(t_ac_rdir_inf *acrd, t_ac_suff_inf *acs
		, int force_file_type)
{
	char	*old_ac_suff;

	if (acs->suff_len == -1)
	{
		old_ac_suff = acs->suff;
		acs->suff_len = ft_strlen(acrd->cur_file_name + acrd->file_word_len);
		acs->suff = (char*)malloc(sizeof(char) * (acs->suff_len + 1));
		if (acs->suff == NULL)
		{
			acs->suff = old_ac_suff;
			acs->suff_len = -1;
			return (0);
		}
		free(old_ac_suff);
		ft_memcpy(acs->suff, acrd->cur_file_name + acrd->file_word_len
				, acs->suff_len + 1);
		acs->suff_type = (force_file_type || !S_ISDIR(acrd->stat_buf.st_mode))
			? ACS_TYPE_FILE : ACS_TYPE_DIR;
		return (1);
	}
	acs->suff_type = ACS_TYPE_NOTHING;
	acs->suff_len = count_same_char(acrd->cur_file_name + acrd->file_word_len
			, acs->suff);
	acs->suff[acs->suff_len] = '\0';
	return (1);
}

int		try_ac_for_this_file(t_ac_rdir_inf *acrd, t_ac_suff_inf *acs)
{
	t_list	*new_elem;
	int		insert_ret;

	if (valid_file_for_ac(acrd))
	{
		insert_ret = 1;
		if ((new_elem = make_new_choice(acrd)) != NULL)
			insert_ret = strlist_insert_sort(&(acs->choices), new_elem);
		if (insert_ret)
		{
			if (acs->suff_len == -1 || !ft_strnequ(acrd->cur_file_name
						+ acrd->file_word_len, acs->suff, acs->suff_len))
			{
				if (!build_ac_suff(acrd, acs, acrd->force_exec_type))
					return (0);
			}
			else
				acs->suff_type = ACS_TYPE_NOTHING;
		}
	}
	return (1);
}

void	autocomplete_with_infs(t_ac_rdir_inf *acrd, t_ac_suff_inf *acs)
{
	if ((acrd->dir = opendir(acrd->dir_to_use)) != NULL)
	{
		while (readdir_to_dirent(acrd, acs))
		{
			if (!try_ac_for_this_file(acrd, acs))
			{
				ft_memdel((void**)&(acrd->cur_file_path));
				break ;
			}
			ft_memdel((void**)&(acrd->cur_file_path));
		}
	}
}

int		init_ac_suff_inf(t_ac_suff_inf *acs)
{
	acs->choices = NULL;
	acs->suff_type = ACS_TYPE_NOTHING;
	acs->suff_len = -1;
	if ((acs->suff = ft_strdup("")) == NULL)
		return (0);
	return (1);
}

void	*delete_ac_suff_inf(t_ac_suff_inf *acs)
{
	if (acs != NULL)
	{
		ft_lstfree(&(acs->choices));
		free(acs->suff);
		free(acs);
	}
	return (NULL);
}
