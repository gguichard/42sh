/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/20 12:49:35 by tcollard          #+#    #+#             */
/*   Updated: 2019/03/02 14:00:45 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

void		fill_last_elem(char **s, int i, int save, t_ast *new)
{
	if (new->type == NO_TYPE)
		fill_input(s, i, save, new);
	else if (i != save)
		add_input_prev_cmd(s, i, save, new);
}

void		link_new_node(t_ast **sort, t_ast *tmp, t_ast *node)
{
	t_ast	*h_node;

	h_node = node;
	if ((*sort)->type == LOGIC)
	{
		if (node->right)
		{
			tmp->left = node->right;
			node->right->back = tmp;
		}
		node->right = tmp;
		tmp->back = node;
	}
	else if (tmp->type != HEREDOC)
	{
		tmp->left = node;
		node->back = tmp;
		((*sort)->type != LOGIC) ? *sort = tmp : 0;
	}
	else
	{
		tmp->left = *sort;
		(*sort)->back = tmp;
		*sort = tmp;
	}
}

static void	fill_last_input(char **s, int end, int start, t_ast *elem)
{
	int		i;
	t_ast	*tmp;
	int		len;

	i = 0;
	tmp = NULL;
	len = end - start + 1;
	if (!(elem->input = (char**)malloc(sizeof(char*) * len)))
		ft_exit_malloc();
	while (start < end && i < len - 1)
	{
		if (!(elem->input[i] = ft_strdup(s[start])))
			ft_exit_malloc();
		start += 1;
		i += 1;
	}
	elem->input[i] = NULL;
	elem->type = CMD;
}

static void	fill_input_prev_cmd(t_ast *elem, int start, int end, char **s)
{
	int		i;
	int		len;
	char	**tmp;

	i = 0;
	len = 0;
	while (elem->input[len])
		len += 1;
	len += end - start;
	(!(tmp = (char**)malloc(sizeof(char*) * (len + 1)))) ? ft_exit_malloc() : 0;
	while (elem->input[i])
	{
		tmp[i] = ft_strdup(elem->input[i]);
		i += 1;
	}
	while (i < len)
	{
		tmp[i] = ft_strdup(s[start++]);
		i += 1;
	}
	tmp[i] = NULL;
	delete_str_tab(elem->input);
	elem->input = tmp;
}

void		add_input_prev_cmd(char **s, int end, int start, t_ast *elem)
{
	if (end == start)
		return ;
	while (elem->back && elem->type != CMD && elem->type < OPERATOR)
		elem = elem->back;
	if (elem->type != CMD)
	{
		while (elem->next)
			elem = elem->next;
		elem = add_new_elem(&elem);
		return (fill_last_input(s, end, start, elem));
	}
	fill_input_prev_cmd(elem, start, end, s);
}
