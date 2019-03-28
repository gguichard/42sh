#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include "libft.h"
#include "shell.h"
#include "hashtable.h"
#include "vars.h"
#include "autocomplete.h"

void	check_for_var_ac(const char *word, t_ac_rdir_inf *acrd
		, t_ac_suff_inf *acs, t_list *var_lst)
{
	if (!init_ac_rdir(word, acrd, 1, DTYPE_NOT_A_DIR))
		return ;
	ft_bzero(&(acrd->stat_buf), sizeof(struct stat));
	acrd->force_exec_type = 1;
	while (var_lst != NULL)
	{
		acrd->cur_file_name = ((t_var*)var_lst->content)->key;
		if (!try_ac_for_this_file(acrd, acs))
			break ;
		var_lst = var_lst->next;
	}
	acrd->force_exec_type = 0;
	delete_ac_rdir(acrd);
}

void	check_for_builtin_ac(const char *word, t_ac_rdir_inf *acrd
		, t_ac_suff_inf *acs, const t_builtin *builtin_tab)
{
	if (!init_ac_rdir(word, acrd, 1, DTYPE_NOT_A_DIR))
		return ;
	ft_bzero(&(acrd->stat_buf), sizeof(struct stat));
	acrd->force_exec_type = 1;
	while (builtin_tab->name != NULL)
	{
		acrd->cur_file_name = builtin_tab->name;
		if (!try_ac_for_this_file(acrd, acs))
			break ;
		++builtin_tab;
	}
	acrd->force_exec_type = 0;
	delete_ac_rdir(acrd);
}

void	check_for_alias_ac(const char *word, t_ac_rdir_inf *acrd
		, t_ac_suff_inf *acs, t_hashtable *aliastable)
{
	t_list	*hashentry_lst;
	size_t	bucket_idx;

	if (!init_ac_rdir(word, acrd, 1, DTYPE_NOT_A_DIR))
		return ;
	ft_bzero(&(acrd->stat_buf), sizeof(struct stat));
	acrd->force_exec_type = 1;
	bucket_idx = 0;
	while (bucket_idx < aliastable->bucket_count)
	{
		hashentry_lst = aliastable->buckets[bucket_idx];
		while (hashentry_lst != NULL)
		{
			acrd->cur_file_name = ((t_hashentry*)hashentry_lst->content)->key;
			if (!try_ac_for_this_file(acrd, acs))
				break ;
			hashentry_lst = hashentry_lst->next;
		}
		++bucket_idx;
	}
	acrd->force_exec_type = 0;
	delete_ac_rdir(acrd);
}

void	check_for_user_ac(const char *word, t_ac_rdir_inf *acrd
		, t_ac_suff_inf *acs)
{
	struct passwd	*user;

	if (!init_ac_rdir(word, acrd, 1, DTYPE_NOT_A_DIR))
		return ;
	ft_bzero(&(acrd->stat_buf), sizeof(struct stat));
	acrd->force_exec_type = 1;
	setpwent();
	while ((user = getpwent()) != NULL)
	{
		acrd->cur_file_name = ft_strdup(user->pw_name);
		if (!try_ac_for_this_file(acrd, acs))
			break ;
	}
	endpwent();
	acrd->force_exec_type = 0;
	delete_ac_rdir(acrd);
}
