/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd_token.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:26:26 by fwerner           #+#    #+#             */
/*   Updated: 2019/04/08 13:26:28 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "str_cmd_inf.h"
#include "split_cmd_token.h"

/*
** Init tout ce qui n'est pas lie aux alias.
*/

static void				init_base_split_cmd(t_split_cmd_inf *sp_cmd
		, t_str_cmd_inf *str_cmd_inf, t_hashtable *aliastable)
{
	ft_bzero(sp_cmd, sizeof(t_split_cmd_inf));
	sp_cmd->scmd = str_cmd_inf;
	sp_cmd->last_char_was_spe = 1;
	sp_cmd->tk_start = str_cmd_inf->str;
	sp_cmd->cur_tk_type = TK_NOTHING;
	sp_cmd->aliastable = aliastable;
}

t_list					*split_cmd_token_with_lst(t_str_cmd_inf *str_cmd_inf
		, t_hashtable *aliastable, t_list *cur_forbidden_aliases
		, size_t cur_alias_recur_lvl)
{
	t_split_cmd_inf		sp_cmd;

	init_base_split_cmd(&sp_cmd, str_cmd_inf, aliastable);
	sp_cmd.forbidden_aliases = cur_forbidden_aliases;
	sp_cmd.alias_recur_lvl = cur_alias_recur_lvl;
	return (internal_split_cmd_token(&sp_cmd));
}

t_list					*split_cmd_token_without_last_alias(
		t_str_cmd_inf *str_cmd_inf, t_hashtable *aliastable)
{
	t_split_cmd_inf		sp_cmd;

	init_base_split_cmd(&sp_cmd, str_cmd_inf, aliastable);
	sp_cmd.forbidden_aliases = NULL;
	sp_cmd.alias_recur_lvl = 0;
	sp_cmd.disable_last_alias_expand = 1;
	return (internal_split_cmd_token(&sp_cmd));
}

t_list					*split_cmd_token(t_str_cmd_inf *str_cmd_inf
		, t_hashtable *aliastable)
{
	t_split_cmd_inf		sp_cmd;

	init_base_split_cmd(&sp_cmd, str_cmd_inf, aliastable);
	sp_cmd.forbidden_aliases = NULL;
	sp_cmd.alias_recur_lvl = 0;
	return (internal_split_cmd_token(&sp_cmd));
}
