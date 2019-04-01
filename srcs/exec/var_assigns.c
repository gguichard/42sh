#include <stdlib.h>
#include "shell.h"
#include "execution.h"
#include "vars.h"

static void	update_var_or_create_local(t_list **vars, const char *key
		, const char *value, int is_tmp)
{
	t_var	*var;

	var = get_var(*vars, key);
	if (var == NULL)
		create_var(vars, key, value, is_tmp ? 2 : 0);
	else if (is_tmp && var->is_env != 2)
	{
		free(var->tmp_value);
		var->tmp_value = ft_strdup(value);
	}
	else
	{
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
			update_var_or_create_local(&alloc->vars, elem->input[idx], tmp + 1
					, is_tmp);
			*tmp = '=';
		}
		idx++;
	}
}

static void	del_tmp_vars(t_alloc *alloc)
{
	t_list	*cur;
	t_list	*prev;
	t_list	*next;

	cur = alloc->vars;
	prev = NULL;
	while (cur != NULL)
	{
		next = cur->next;
		if (((t_var *)cur->content)->is_env != 2
				&& ((t_var *)cur->content)->tmp_value == NULL)
			prev = cur;
		else if (((t_var *)cur->content)->is_env != 2)
			ft_strdel(&((t_var *)cur->content)->tmp_value);
		else
		{
			ft_lstdelone(&cur, free_var);
			if (prev == NULL)
				alloc->vars = next;
			else
				prev->next = next;
		}
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
	del_tmp_vars(alloc);
	return (ret);
}
