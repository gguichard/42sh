#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "autocomplete.h"

void	autocomplete_from_wordpath(const char *word, int is_a_cmd
		, t_dir_type dir_type, t_ac_suff_inf *acs)
{
	t_ac_rdir_inf	acrd;

	if (!init_ac_rdir(word, &acrd, is_a_cmd, dir_type))
	{
		ft_memdel((void**)&(acs->suff));
		return ;
	}
	autocomplete_with_infs(&acrd, acs);
	delete_ac_rdir(&acrd);
}

char	*build_path_to_file(const char *path, const char *file)
{
	char	*path_to_file;

	if ((path_to_file = (char*)malloc(sizeof(char) * (ft_strlen(path)
						+ ft_strlen(file) + 2))) == NULL)
		return (NULL);
	path_to_file[0] = '\0';
	ft_strcat(path_to_file, path);
	ft_strcat(path_to_file, "/");
	ft_strcat(path_to_file, file);
	return (path_to_file);
}

void	autocomplete_cmd(const char *word, char **path_tab, t_alloc *alloc
		, t_ac_suff_inf *acs)
{
	t_ac_rdir_inf	acrd;
	char			*real_word;

	check_for_builtin_ac(word, &acrd, acs, alloc->builtins);
	check_for_alias_ac(word, &acrd, acs, alloc->aliastable);
	while (*path_tab != NULL)
	{
		if ((real_word = build_path_to_file(*path_tab, word)) == NULL
				|| !init_ac_rdir(real_word, &acrd, 1, DTYPE_NOT_A_DIR))
		{
			free(real_word);
			ft_memdel((void**)&(acs->suff));
			return ;
		}
		autocomplete_with_infs(&acrd, acs);
		free(real_word);
		delete_ac_rdir(&acrd);
		++path_tab;
	}
	if (acs->choices == NULL)
		autocomplete_from_wordpath(word, 0, DTYPE_IS_A_DIR, acs);
}
