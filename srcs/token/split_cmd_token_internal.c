/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd_token_internal.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:26:43 by fwerner           #+#    #+#             */
/*   Updated: 2019/04/16 12:42:02 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "str_cmd_inf.h"
#include "token_inf.h"
#include "split_cmd_token.h"

/*
** Ajoute les tokens avant le char actuel dans la liste dans le cas ou le char
** actuel est un spe char.
*/

static int		process_token_before_cur_char(t_split_cmd_inf *sp_cmd)
{
	size_t	old_pos;

	old_pos = sp_cmd->scmd->pos;
	sp_cmd->cur_tk_type = get_tk_type_before_cur_char(sp_cmd);
	scmd_move_pos(sp_cmd->scmd, -1);
	if (sp_cmd->last_tk_end_by_and)
		scmd_move_pos(sp_cmd->scmd, -1);
	if ((sp_cmd->scmd->str + sp_cmd->scmd->pos) >= sp_cmd->tk_start
			&& !add_cur_token_to_lst(sp_cmd))
	{
		scmd_set_pos(sp_cmd->scmd, old_pos);
		return (0);
	}
	if (sp_cmd->alias_has_expanded)
		return (1);
	scmd_set_pos(sp_cmd->scmd, old_pos);
	if (sp_cmd->last_tk_end_by_and)
	{
		if (!add_whole_token_to_lst(sp_cmd, "&", TK_RED_LOPT_FD))
			return (0);
	}
	return (1);
}

/*
** Ajoute un token dans la liste des token en fonction du char actuel. Ajoute
** d'autres tokens pour les caracteres precedent si besoin.
*/

static int		split_spe_char(t_split_cmd_inf *sp_cmd)
{
	if (!sp_cmd->last_char_was_spe)
		if (!process_token_before_cur_char(sp_cmd))
			return (0);
	if (sp_cmd->alias_has_expanded)
		return (1);
	sp_cmd->tk_start = sp_cmd->scmd->str + sp_cmd->scmd->pos;
	if (ft_strchr(WORD_SEP_CHARS, sp_cmd->scmd->str[sp_cmd->scmd->pos]) == NULL)
	{
		scmd_move_pos(sp_cmd->scmd, get_cur_spe_char_token_len_and_set_type(
					sp_cmd) - 1);
		if (!add_cur_token_to_lst(sp_cmd))
			return (0);
	}
	return (1);
}

/*
** Si le char actuel est un char special, ajoute ce char a la liste des tokens.
** sinon prend le char actuel comme debut de token si le dernier char etait un
** char special.
*/

static int		split_at_pos(t_split_cmd_inf *sp_cmd)
{
	if (scmd_cur_char_is_in_nothing(sp_cmd->scmd)
			&& is_a_spe_char(sp_cmd->scmd->str[sp_cmd->scmd->pos])
			&& !scmd_cur_char_is_escaped(sp_cmd->scmd)
			&& (sp_cmd->scmd->str[sp_cmd->scmd->pos] != '&'
				|| sp_cmd->scmd->str[sp_cmd->scmd->pos + 1] != '>'))
	{
		if (!split_spe_char(sp_cmd))
			return (0);
		if (sp_cmd->alias_has_expanded)
			return (1);
		sp_cmd->last_char_was_spe = 1;
	}
	else
	{
		sp_cmd->cur_tk_type = get_next_word_tk_type(sp_cmd);
		if (sp_cmd->last_char_was_spe)
			sp_cmd->tk_start = sp_cmd->scmd->str + sp_cmd->scmd->pos;
		sp_cmd->last_char_was_spe = 0;
	}
	return (1);
}

/*
** Retourne toujours NULL.
*/

static void		*end_sp_cmd_inf(t_split_cmd_inf *sp_cmd
		, char because_of_error)
{
	scmd_set_pos(sp_cmd->scmd, ft_strlen(sp_cmd->scmd->str));
	if (because_of_error)
		ft_lstdel(&sp_cmd->tk_lst, del_token);
	return (NULL);
}

t_list			*internal_split_cmd_token(t_split_cmd_inf *sp_cmd)
{
	if (sp_cmd->alias_recur_lvl > 500)
		return (NULL);
	while (1)
	{
		sp_cmd->alias_has_expanded = 0;
		while (sp_cmd->scmd->str[sp_cmd->scmd->pos] != '\0')
		{
			if (!split_at_pos(sp_cmd))
				return (end_sp_cmd_inf(sp_cmd, 1));
			sp_cmd->alias_has_expanded = 0;
			scmd_move_to_next_char(sp_cmd->scmd);
		}
		if (sp_cmd->disable_last_alias_expand)
			sp_cmd->pos_alias_can_start = -1;
		if (sp_cmd->scmd->pos > 0)
			scmd_move_pos(sp_cmd->scmd, -1);
		else
			sp_cmd->cur_tk_type = TK_NOTHING;
		if (sp_cmd->cur_tk_type != TK_NOTHING && !add_cur_token_to_lst(sp_cmd))
			return (end_sp_cmd_inf(sp_cmd, 1));
		if (!sp_cmd->alias_has_expanded)
			break ;
	}
	end_sp_cmd_inf(sp_cmd, 0);
	return (sp_cmd->tk_lst);
}
