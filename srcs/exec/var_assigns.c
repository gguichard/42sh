#include <stdlib.h>
#include "shell.h"
#include "vars.h"

static void	update_var_or_create_local(t_list **vars, const char *key
		, const char *value)
{
	t_var	*var;

	var = get_var(*vars, key);
	if (var == NULL)
		create_var(vars, key, value, 0);
	else
	{
		free(var->value);
		var->value = ft_strdup(value);
	}
}

static void	assign_local_vars(t_alloc *alloc, t_ast *elem)
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
			update_var_or_create_local(&alloc->vars, elem->input[idx], tmp + 1);
			*tmp = '=';
		}
		idx++;
	}
}

void		process_assigns(t_alloc *alloc, t_ast *elem)
{
	if (elem->left == NULL)
		assign_local_vars(alloc, elem);
}
