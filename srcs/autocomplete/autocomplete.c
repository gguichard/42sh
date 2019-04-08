#include <stdlib.h>
#include "libft.h"
#include "convert_path_to_tab.h"
#include "shell.h"
#include "autocomplete.h"

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
			autocomplete_from_wordpath(word, is_a_cmd, DTYPE_MAY_BE_DIR, acs);
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

t_ac_suff_inf	*autocomplete_user(const char *word)
{
	t_ac_suff_inf	*acs;
	t_ac_rdir_inf	acrd;

	if ((acs = (t_ac_suff_inf*)malloc(sizeof(t_ac_suff_inf))) == NULL)
		return (NULL);
	if (init_ac_suff_inf(acs))
	{
		check_for_user_ac(word, &acrd, acs);
		if (acs->suff != NULL)
			return (acs);
	}
	return (delete_ac_suff_inf(acs));
}
