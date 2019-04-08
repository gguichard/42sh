/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ac_cmdline.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 11:49:37 by fwerner           #+#    #+#             */
/*   Updated: 2019/04/08 11:49:39 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"
#include "token_inf.h"
#include "autocomplete.h"

/*
** Alloue et retourne une nouvelle string representant la version inhibe de
** str. Retourne NULL en cas d'erreur.
*/

static char				*inhibe_this_str_for_autocomplete(const char *str
		, t_alloc *alloc, int *has_several_words)
{
	char	**str_tab;
	char	**tmp_tab;
	char	*new_str;

	*has_several_words = 0;
	new_str = NULL;
	str_tab = inhib_expand_str(str, alloc);
	tmp_tab = str_tab;
	while (tmp_tab != NULL && *tmp_tab != NULL)
	{
		if (new_str != NULL)
			*has_several_words = 1;
		new_str = *tmp_tab;
		++tmp_tab;
	}
	if (new_str == NULL)
		new_str = ft_strdup(str);
	else
		new_str = ft_strdup(new_str);
	ft_strtab_free(str_tab);
	return (new_str);
}

static t_ac_suff_inf	*autocomplete_cmdline_not_var_assign(t_token_inf *cur_tk
		, t_alloc *alloc, const char *real_start)
{
	char			*inhibed_str;
	const char		*tmp_start;
	t_ac_suff_inf	*acs_inf;
	int				has_several_words;

	if ((tmp_start = find_last_home_user(real_start)) != NULL)
	{
		inhibed_str = inhibe_this_str_for_autocomplete(tmp_start, alloc
				, &has_several_words);
		acs_inf = autocomplete_user(inhibed_str == NULL ? "" : inhibed_str);
		if (acs_inf != NULL && acs_inf->suff_type == ACS_TYPE_FILE)
			acs_inf->suff_type = ACS_TYPE_DIR;
	}
	else
	{
		inhibed_str = inhibe_this_str_for_autocomplete(real_start, alloc
				, &has_several_words);
		acs_inf = autocomplete_word(alloc->vars, (inhibed_str == NULL
					? "" : inhibed_str)
				, cur_tk->type == TK_CMD && !has_several_words, alloc);
	}
	free(inhibed_str);
	return (acs_inf);
}

static t_ac_suff_inf	*autocomplete_cmdline_not_var(t_token_inf *cur_tk
		, t_alloc *alloc)
{
	const char		*real_start;
	const char		*tmp_start;

	real_start = (cur_tk->token == NULL ? "" : cur_tk->token);
	tmp_start = NULL;
	if (cur_tk->type != TK_RED_ROPT_FILE)
	{
		if ((tmp_start = find_last_assign_start(real_start)) != NULL)
			real_start = tmp_start;
	}
	return (autocomplete_cmdline_not_var_assign(cur_tk, alloc, real_start));
}

t_ac_suff_inf			*autocomplete_cmdline(t_str_cmd_inf *scmd
		, t_alloc *alloc)
{
	t_token_inf		cur_tk;
	t_ac_suff_inf	*acs_inf;
	const char		*real_ac_start;
	int				is_in_bracket;

	set_cur_token_cmd(&cur_tk, scmd, alloc);
	real_ac_start = find_last_var_start(cur_tk.token, &is_in_bracket);
	if (real_ac_start == NULL)
	{
		acs_inf = autocomplete_cmdline_not_var(&cur_tk, alloc);
	}
	else
	{
		acs_inf = autocomplete_var(alloc->vars, real_ac_start);
		if (acs_inf != NULL && acs_inf->suff_type == ACS_TYPE_FILE
				&& is_in_bracket)
			acs_inf->suff_type = ACS_TYPE_VAR_IN_BRACKETS;
	}
	free(cur_tk.token);
	return (acs_inf);
}
