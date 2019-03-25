#include <stdlib.h>
#include "libft.h"
#include "str_cmd_inf.h"
#include "aliastable.h"
#include "token_inf.h"
#include "split_cmd_token.h"

static int	update_sp_cmd_with_inf(t_split_cmd_inf *sp_cmd
		, t_list *new_elem)
{
	if (sp_cmd->pos_alias_can_start <= sp_cmd->tk_start - sp_cmd->scmd->str)
	{
		sp_cmd->pos_alias_can_start = -1;
		if (sp_cmd->cur_tk_type == TK_CMD_SEP)
			sp_cmd->pos_alias_can_start = 0;
		sp_cmd->last_alias_ended_with_blank = 0;
	}
	sp_cmd->last_tk_added = new_elem;
	if (get_tk(new_elem)->type == TK_CMD_SEP)
		sp_cmd->last_start_cmd = NULL;
	else if (sp_cmd->last_start_cmd == NULL)
		sp_cmd->last_start_cmd = new_elem;
	sp_cmd->cur_tk_type = TK_NOTHING;
	return (1);
}

static int	add_token_to_lst(t_split_cmd_inf *sp_cmd, const char *token_str
		, size_t token_size, t_token_type token_type)
{
	t_token_inf		new_token;
	t_list			*new_elem;

	if (token_type == TK_NOTHING)
		return (1);
	new_token.token = ft_strndup(token_str, token_size);
	new_token.type = token_type;
	if (new_token.token == NULL
			|| (new_elem = ft_lstnew(&new_token, sizeof(t_token_inf))) == NULL)
	{
		free(new_token.token);
		sp_cmd->cur_tk_type = TK_NOTHING;
		return (0);
	}
	if (sp_cmd->last_tk_added == NULL)
		ft_lstpush(&sp_cmd->tk_lst, new_elem);
	else
		ft_lstpush(&sp_cmd->last_tk_added, new_elem);
	return (update_sp_cmd_with_inf(sp_cmd, new_elem));
}

static int	token_is_assign(const char *token, size_t token_size)
{
	size_t	idx;

	idx = 0;
	while (idx < token_size)
	{
		if (!ft_isalpha(token[idx])
				&& (!ft_isdigit(token[idx]) || idx == 0)
				&& token[idx] != '_')
			break ;
		++idx;
	}
	return (token[idx] == '=');
}

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

static int	expand_alias(t_split_cmd_inf *sp_cmd)
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

int			add_cur_token_to_lst(t_split_cmd_inf *sp_cmd)
{
	if (sp_cmd->cur_tk_type == TK_CMD
			&& token_is_assign(sp_cmd->tk_start
				, scmd_cur_str(sp_cmd->scmd) - sp_cmd->tk_start + 1))
		sp_cmd->cur_tk_type = TK_ASSIGN;
	if (sp_cmd->pos_alias_can_start != -1
			&& sp_cmd->pos_alias_can_start
			<= sp_cmd->tk_start - sp_cmd->scmd->str
			&& (sp_cmd->cur_tk_type == TK_CMD
				|| (sp_cmd->cur_tk_type == TK_PARAM
					&& sp_cmd->last_alias_ended_with_blank))
			&& expand_alias(sp_cmd))
		return (1);
	return (add_token_to_lst(sp_cmd, sp_cmd->tk_start
				, scmd_cur_str(sp_cmd->scmd) - sp_cmd->tk_start + 1
				, sp_cmd->cur_tk_type));
}

int			add_whole_token_to_lst(t_split_cmd_inf *sp_cmd
		, const char *token_str, t_token_type token_type)
{
	return (add_token_to_lst(sp_cmd, token_str, ft_strlen(token_str)
				, token_type));
}
