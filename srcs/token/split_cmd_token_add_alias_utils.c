/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd_token_add_alias_utils.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:26:40 by fwerner           #+#    #+#             */
/*   Updated: 2019/04/08 13:26:41 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "str_cmd_inf.h"
#include "aliastable.h"
#include "token_inf.h"
#include "split_cmd_token.h"

static int	init_alias_scmd_and_elem(t_str_cmd_inf *alias_scmd
		, t_split_cmd_inf *sp_cmd, t_list *cur_alias_elem)
{
	t_list	*tmp_tk_lst;
	t_list	*cur_fordbidden_alias;

	cur_alias_elem->next = sp_cmd->forbidden_aliases;
	if ((cur_alias_elem->content = ft_strndup(sp_cmd->tk_start
					, sp_cmd->scmd->str + sp_cmd->scmd->pos
					- sp_cmd->tk_start + 1)) == NULL)
		return (0);
	cur_fordbidden_alias = sp_cmd->forbidden_aliases;
	while (cur_fordbidden_alias != NULL)
	{
		if (ft_strequ(cur_fordbidden_alias->content, cur_alias_elem->content))
			return (!!ft_memdel(&cur_alias_elem->content));
		cur_fordbidden_alias = cur_fordbidden_alias->next;
	}
	if (!scmd_init(alias_scmd, get_alias(sp_cmd->aliastable
					, cur_alias_elem->content)) || alias_scmd->str == NULL)
	{
		return (!!ft_memdel(&cur_alias_elem->content));
	}
	if ((tmp_tk_lst = split_cmd_token_with_lst(alias_scmd
					, sp_cmd->aliastable, cur_alias_elem
					, sp_cmd->alias_recur_lvl + 1)) != NULL)
		ft_lstdel(&tmp_tk_lst, del_token);
	return (1);
}

static int	replace_token_with_alias_res_and_clean(t_split_cmd_inf *sp_cmd
		, size_t tk_start_pos, t_str_cmd_inf *alias_scmd
		, size_t alias_name_size)
{
	if (!ft_strreplace_inside(&sp_cmd->scmd->str, tk_start_pos, alias_name_size
				, alias_scmd->str))
	{
		scmd_clean(alias_scmd);
		return (0);
	}
	scmd_clean(alias_scmd);
	sp_cmd->scmd->pos = tk_start_pos;
	sp_cmd->tk_start = sp_cmd->scmd->str + tk_start_pos;
	sp_cmd->alias_has_expanded = 1;
	return (1);
}

/*
** Retourne 1 si un expand a eu lieu.
*/

int			expand_alias(t_split_cmd_inf *sp_cmd)
{
	t_str_cmd_inf	alias_scmd;
	size_t			alias_name_size;
	size_t			alias_result_size;
	size_t			tk_start_pos;
	t_list			cur_alias_elem;

	ft_bzero(&cur_alias_elem, sizeof(t_list));
	if (!init_alias_scmd_and_elem(&alias_scmd, sp_cmd, &cur_alias_elem))
		return (0);
	alias_name_size = sp_cmd->scmd->str + sp_cmd->scmd->pos
		- sp_cmd->tk_start + 1;
	alias_result_size = ft_strlen(alias_scmd.str);
	tk_start_pos = sp_cmd->tk_start - sp_cmd->scmd->str;
	sp_cmd->last_alias_ended_with_blank = (alias_result_size > 0
			&& (alias_scmd.str[alias_result_size - 1] == ' '
				|| alias_scmd.str[alias_result_size - 1] == '\t'));
	sp_cmd->pos_alias_can_start = tk_start_pos + alias_result_size;
	free(cur_alias_elem.content);
	return (replace_token_with_alias_res_and_clean(sp_cmd, tk_start_pos
				, &alias_scmd, alias_name_size));
}
