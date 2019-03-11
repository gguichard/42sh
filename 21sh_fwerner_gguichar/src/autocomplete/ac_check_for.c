/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ac_check_for.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/30 08:15:12 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/30 08:19:36 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <sys/stat.h>
#include "libft.h"
#include "vars.h"
#include "autocomplete.h"

void	check_for_var_ac(const char *word, t_ac_rdir_inf *acrd
		, t_ac_suff_inf *acs, t_list *var_lst)
{
	if (!init_ac_rdir(word, acrd, 1, 0))
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
		, t_ac_suff_inf *acs, t_builtin *builtin_tab)
{
	if (!init_ac_rdir(word, acrd, 1, 0))
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
