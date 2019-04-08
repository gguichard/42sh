/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ac_cmdline_tk.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 11:49:41 by fwerner           #+#    #+#             */
/*   Updated: 2019/04/08 11:49:43 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "split_cmd_token.h"
#include "str_cmd_inf.h"
#include "token_inf.h"
#include "autocomplete.h"

static void		fill_cur_tk_with_new_token(t_token_inf *cur_tk
		, t_str_cmd_inf *scmd, t_alloc *alloc)
{
	char			*new_str;
	t_str_cmd_inf	new_scmd;
	t_list			*tk_list;
	t_list			*last_tk;

	cur_tk->type = TK_NOTHING;
	cur_tk->token = NULL;
	new_str = ft_strjoin(scmd->str, " H");
	if (!scmd_init(&new_scmd, new_str) || new_str == NULL)
	{
		free(new_str);
		scmd_clean(&new_scmd);
		return ;
	}
	free(new_str);
	tk_list = split_cmd_token_without_last_alias(&new_scmd, alloc->aliastable);
	last_tk = tk_list;
	while (last_tk != NULL && last_tk->next != NULL)
		last_tk = last_tk->next;
	if (last_tk != NULL)
		cur_tk->type = get_tk(last_tk)->type;
	ft_lstdel(&tk_list, del_token);
	scmd_clean(&new_scmd);
}

static size_t	get_pos_last_sub_cmd_start(t_str_cmd_inf *scmd)
{
	int		last_start_found;
	size_t	last_sub_cmd_start_pos;

	last_start_found = 0;
	last_sub_cmd_start_pos = 0;
	while (1)
	{
		if (!last_start_found && scmd->sub_str_cmd != NULL
				&& scmd->sub_str_cmd->cur_str_cmd_type == SCMD_TYPE_SUBCMD)
		{
			last_sub_cmd_start_pos = scmd->pos;
			last_start_found = 1;
		}
		else if (last_start_found && (scmd->sub_str_cmd == NULL
					|| scmd->sub_str_cmd->cur_str_cmd_type != SCMD_TYPE_SUBCMD))
		{
			last_sub_cmd_start_pos = 0;
			last_start_found = 0;
		}
		if (!scmd_move_to_next_char(scmd))
			break ;
	}
	return (last_sub_cmd_start_pos);
}

static void		fill_cur_tk_with_last_token(t_token_inf *cur_tk
		, t_token_inf *last_tk, t_str_cmd_inf *scmd, t_alloc *alloc)
{
	if (scmd->sub_str_cmd != NULL
			&& scmd->sub_str_cmd->cur_str_cmd_type == SCMD_TYPE_SUBCMD)
	{
		if (scmd_reset(scmd, last_tk->token))
		{
			scmd->pos = get_pos_last_sub_cmd_start(scmd);
			if (scmd_reset(scmd, scmd->str + scmd->pos))
			{
				set_cur_token_cmd(cur_tk, scmd, alloc);
				return ;
			}
		}
	}
	cur_tk->type = last_tk->type;
	cur_tk->token = ft_strdup(last_tk->token);
}

void			set_cur_token_cmd(t_token_inf *cur_tk_cmd, t_str_cmd_inf *scmd
		, t_alloc *alloc)
{
	t_list			*tk_list;
	t_list			*last_tk;

	tk_list = split_cmd_token_without_last_alias(scmd, alloc->aliastable);
	last_tk = tk_list;
	while (last_tk != NULL && last_tk->next != NULL)
		last_tk = last_tk->next;
	if (scmd_cur_char_is_in_nothing(scmd) && (scmd->pos == 0 || last_tk == NULL
				|| (scmd_char_at_is_of(scmd, scmd->pos - 1, WORD_SEP_CHARS)
					&& !scmd_char_at_is_escaped(scmd, scmd->pos - 1))
				|| get_tk(last_tk)->type == TK_RED_OPE
				|| get_tk(last_tk)->type == TK_CMD_SEP))
		fill_cur_tk_with_new_token(cur_tk_cmd, scmd, alloc);
	else
		fill_cur_tk_with_last_token(cur_tk_cmd, get_tk(last_tk), scmd, alloc);
	ft_lstdel(&tk_list, del_token);
}
