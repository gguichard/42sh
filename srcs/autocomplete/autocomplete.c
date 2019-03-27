#include <stdlib.h>
#include "libft.h"
#include "convert_path_to_tab.h"
#include "shell.h"
#include "autocomplete.h"

/*
** Remplie le t_ac_suff_inf avec les informations pour autocompleter le word
** passe en parametre, si is_a_cmd est vrai le suffix ne peut etre qu'un
** executable ou un dossier.
*/

static void		autocomplete_from_wordpath(const char *word, int is_a_cmd
		, t_ac_suff_inf *acs)
{
	t_ac_rdir_inf	acrd;

	if (!init_ac_rdir(word, &acrd, is_a_cmd, 1))
	{
		ft_memdel((void**)&(acs->suff));
		return ;
	}
	autocomplete_with_infs(&acrd, acs);
	delete_ac_rdir(&acrd);
}

/*
** Alloue et retourne une string contenant la concatenation de path + / + file.
*/

static char		*build_path_to_file(const char *path, const char *file)
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

/*
** Remplie le t_ac_suff_inf avec les informations pour autocompleter une
** commande builtin, alias, ou se trouvant dans le path.
*/

static void		autocomplete_cmd(const char *word, char **path_tab
		, t_alloc *alloc, t_ac_suff_inf *acs)
{
	t_ac_rdir_inf	acrd;
	char			*real_word;

	check_for_builtin_ac(word, &acrd, acs, alloc->builtins);
	check_for_alias_ac(word, &acrd, acs, alloc->aliastable);
	while (*path_tab != NULL)
	{
		if ((real_word = build_path_to_file(*path_tab, word)) == NULL
				|| !init_ac_rdir(real_word, &acrd, 1, 0))
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
}

t_ac_suff_inf	*autocomplete_word(t_list *var_lst, const char *word
		, int is_a_cmd, t_alloc *alloc)
{
	t_ac_suff_inf	*acs;
	char			**path_tab;

	if ((acs = (t_ac_suff_inf*)malloc(sizeof(t_ac_suff_inf))) == NULL)
		return (NULL);
	if (init_ac_suff_inf(acs))
	{
		if (!is_a_cmd || ft_strchr(word, '/') != NULL)
			autocomplete_from_wordpath(word, is_a_cmd, acs);
		else
		{
			path_tab = convert_path_to_tab(var_lst);
			if (path_tab != NULL)
				autocomplete_cmd(word, path_tab, alloc, acs);
			ft_strtab_free(path_tab);
		}
		if (acs->suff != NULL)
			return (acs);
	}
	return (delete_ac_suff_inf(acs));
}

t_ac_suff_inf	*autocomplete_var(t_list *var_lst, const char *word)
{
	t_ac_suff_inf	*acs;
	t_ac_rdir_inf	acrd;

	if ((acs = (t_ac_suff_inf*)malloc(sizeof(t_ac_suff_inf))) == NULL)
		return (NULL);
	if (init_ac_suff_inf(acs))
	{
		check_for_var_ac(word, &acrd, acs, var_lst);
		if (acs->suff != NULL)
			return (acs);
	}
	return (delete_ac_suff_inf(acs));
}
