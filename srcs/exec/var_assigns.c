/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_assigns.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 19:42:08 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/10 12:06:51 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "execution.h"
#include "hashtable.h"
#include "vars.h"

static void	update_var_or_create_local(t_alloc *alloc, const char *key
		, const char *value, int is_tmp)
{
	t_var	*var;

	var = get_var(alloc->vars, key);
	if (var == NULL)
		create_var(&alloc->vars, key, value, is_tmp ? 2 : 0);
	else
	{
		if (ft_strequ(key, "PATH"))
			delete_hashentries(alloc->exectable);
		if (is_tmp && var->is_env != 2 && var->tmp_value == NULL)
		{
			var->tmp_value = var->value;
			var->value = NULL;
		}
		free(var->value);
		var->value = ft_strdup(value);
	}
}

static void	assign_local_vars(t_alloc *alloc, t_ast *elem, int is_tmp)
{
	int		idx;
	char	*tmp;

	idx = 0;
	while (elem->input[idx] != NULL)
	{
		tmp = ft_strchr(elem->input[idx], '=');
		if (tmp != NULL)
		{
			*tmp = '\0';
			update_var_or_create_local(alloc, elem->input[idx], tmp + 1
					, is_tmp);
			*tmp = '=';
		}
		idx++;
	}
}

static void	reset_tmp_var(t_alloc *alloc, t_list *cur, t_list *next
		, t_list **prev)
{
	t_var	*var;

	var = (t_var *)cur->content;
	if (var->is_env != 2 && var->tmp_value == NULL)
		*prev = cur;
	else if (var->is_env != 2)
	{
		free(var->value);
		var->value = var->tmp_value;
		var->tmp_value = NULL;
	}
	else
	{
		ft_lstdelone(&cur, free_var);
		if (*prev == NULL)
			alloc->vars = next;
		else
			(*prev)->next = next;
	}
}

static void	reset_tmp_vars(t_alloc *alloc)
{
	t_list	*cur;
	t_list	*prev;
	t_list	*next;

	cur = alloc->vars;
	prev = NULL;
	while (cur != NULL)
	{
		next = cur->next;
		reset_tmp_var(alloc, cur, next, &prev);
		cur = next;
	}
}

int			process_assigns(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	int	ret;

	if (elem->left == NULL)
	{
		assign_local_vars(alloc, elem, 0);
		return (analyzer(alloc, elem->left, opt));
	}
	assign_local_vars(alloc, elem, 1);
	ret = analyzer(alloc, elem->left, opt);
	reset_tmp_vars(alloc);
	return (ret);
}
