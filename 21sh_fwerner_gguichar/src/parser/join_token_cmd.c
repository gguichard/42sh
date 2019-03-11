/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join_token_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/18 11:46:53 by fwerner           #+#    #+#             */
/*   Updated: 2019/01/28 17:34:05 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "cmd_inf.h"
#include "token_inf.h"
#include "join_token_cmd.h"

static int	process_tk_ope(t_cmd_inf *cmd_inf, t_list **token_lst
		, t_shell *shell)
{
	if (get_tk(*token_lst)->token[0] == '<'
			|| get_tk(*token_lst)->token[0] == '>')
	{
		if ((*token_lst)->next == NULL
				|| !add_redirect_inf_to_cmd(cmd_inf, 0, *token_lst, shell))
			return (del_cur_cmd(cmd_inf));
		*token_lst = get_ropt_elem((*token_lst)->next);
	}
	else if (ft_strequ(get_tk(*token_lst)->token, "|"))
	{
		if ((cmd_inf->pipe_cmd = (t_cmd_inf*)malloc(sizeof(t_cmd_inf)))
				== NULL)
			return (del_cur_cmd(cmd_inf));
		*token_lst = (*token_lst)->next;
		if (set_cur_cmd(cmd_inf->pipe_cmd, token_lst, shell))
			return (1);
		else
			return (del_cur_cmd(cmd_inf));
	}
	return (-1);
}

static int	process_tk(t_cmd_inf *cmd_inf, t_list **token_lst, t_shell *shell)
{
	if (get_tk(*token_lst)->type == TK_WORD)
	{
		if (!add_arg(cmd_inf, get_tk(*token_lst)->token, shell))
			return (del_cur_cmd(cmd_inf));
	}
	else if (get_tk(*token_lst)->type == TK_OPE)
	{
		return (process_tk_ope(cmd_inf, token_lst, shell));
	}
	else if (get_tk(*token_lst)->type == TK_NUM_OPT)
	{
		if ((*token_lst)->next == NULL || (*token_lst)->next->next == NULL
				|| !add_redirect_inf_to_cmd(cmd_inf, 1, *token_lst, shell))
			return (del_cur_cmd(cmd_inf));
		*token_lst = get_ropt_elem((*token_lst)->next->next);
	}
	else if (get_tk(*token_lst)->type == TK_CMD_SEP)
		return (1);
	return (-1);
}

int			set_cur_cmd(t_cmd_inf *cmd_inf, t_list **token_lst, t_shell *shell)
{
	int		tmp_ret;

	cmd_inf->arg_lst = NULL;
	cmd_inf->pipe_cmd = NULL;
	cmd_inf->redirect_lst = NULL;
	while (*token_lst != NULL)
	{
		if ((tmp_ret = process_tk(cmd_inf, token_lst, shell)) != -1)
			return (tmp_ret);
		if (*token_lst != NULL)
			*token_lst = (*token_lst)->next;
	}
	return (1);
}

t_list		*join_token_cmd(t_list *token_lst, t_shell *shell)
{
	t_list		*cmd_lst;
	t_list		*new_elem;
	t_cmd_inf	cur_cmd;

	cmd_lst = NULL;
	while (token_lst != NULL)
	{
		if (!set_cur_cmd(&cur_cmd, &token_lst, shell))
		{
			ft_lstdel(&cmd_lst, del_cmd);
			return (NULL);
		}
		else
		{
			if ((new_elem = ft_lstnew(&cur_cmd, sizeof(t_cmd_inf))) == NULL)
			{
				ft_lstdel(&cmd_lst, del_cmd);
				return (NULL);
			}
			ft_lstpush(&cmd_lst, new_elem);
		}
		if (token_lst != NULL)
			token_lst = token_lst->next;
	}
	return (cmd_lst);
}
