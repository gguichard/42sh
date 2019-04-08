#include <stdlib.h>
#include "libft.h"
#include "str_cmd_inf.h"
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
	return (idx > 0 && token[idx] == '=');
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
				|| ((sp_cmd->cur_tk_type == TK_PARAM
						|| sp_cmd->cur_tk_type == TK_RED_ROPT_FILE)
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
