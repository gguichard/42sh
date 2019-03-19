/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd_token_add.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fwerner <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/29 10:50:30 by fwerner           #+#    #+#             */
/*   Updated: 2019/03/19 16:28:00 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "str_cmd_inf.h"
#include "token_inf.h"
#include "split_cmd_token.h"

int		add_cur_token_to_lst(t_list **token_lst, t_str_cmd_inf *str_cmd_inf
		, const char *token_start
		, t_token_type token_type)
{
	t_token_inf		new_token;
	t_list			*new_elem;
	size_t			cur_token_size;

	if (token_type == TK_NOTHING)
		return (1);
	cur_token_size = (str_cmd_inf->str + str_cmd_inf->pos) - token_start
		+ (token_type == TK_OPE || token_type == TK_CMD_SEP ? 1 : 0);
	new_token.token = ft_strndup(token_start, cur_token_size);
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

int		add_token_to_lst(t_list **token_lst, char *token_str
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
