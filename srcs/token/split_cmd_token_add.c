#include <stdlib.h>
#include "libft.h"
#include "str_cmd_inf.h"
#include "token_inf.h"
#include "split_cmd_token.h"

int		add_cur_token_to_lst(t_split_cmd_inf *sp_cmd)
{
	t_token_inf		new_token;
	t_list			*new_elem;
	size_t			cur_token_size;

	if (sp_cmd->cur_tk_type == TK_NOTHING)
		return (1);
	cur_token_size = (sp_cmd->scmd->str + sp_cmd->scmd->pos)
		- sp_cmd->tk_start + 1;
	new_token.token = ft_strndup(sp_cmd->tk_start, cur_token_size);
	new_token.type = sp_cmd->cur_tk_type;
	if (new_token.token == NULL
			|| (new_elem = ft_lstnew(&new_token, sizeof(t_token_inf))) == NULL)
	{
		free(new_token.token);
		return (0);
	}
	ft_lstpush(&sp_cmd->tk_lst, new_elem);
	return (1);
}

int		add_token_to_lst(t_list **token_lst, const char *token_str
		, t_token_type token_type)
{
	t_token_inf		new_token;
	t_list			*new_elem;

	if (token_type == TK_NOTHING)
		return (1);
	new_token.token = ft_strdup(token_str);
	new_token.type = token_type;
	if (new_token.token == NULL
			|| (new_elem = ft_lstnew(&new_token, sizeof(t_token_inf))) == NULL)
	{
		free(new_token.token);
		return (0);
	}
	ft_lstpush(token_lst, new_elem);
	return (1);
}
