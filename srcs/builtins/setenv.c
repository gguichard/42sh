#include "shell.h"
#include "builtins.h"
#include "error.h"

int	setenv_builtins(t_ast *elem, t_var **lst_env, t_alloc *alloc)
{
	t_var *tmp;

	(void)alloc;
	tmp = *lst_env;
	if (elem->input[1] && ft_strchr(elem->input[1], '='))
		return (error_setenv(2));
	else if (!elem->input[1] || !elem->input[2])
		return (error_setenv(1));
	else if (elem->input[3] && elem->input[4])
		return (error_setenv(3));
	while (tmp)
	{
		if (ft_strcmp(elem->input[1], tmp->key) == 0)
			break ;
		tmp = tmp->next;
	}
	if (tmp && elem->input[3] && ft_atoi(elem->input[3]) != 0)
	{
		free(tmp->value);
		(!(tmp->value = ft_strdup(elem->input[2]))) ? ft_exit_malloc() : 0;
	}
	else if (!tmp)
		add_elem_env(lst_env, elem->input[1], elem->input[2]);
	g_ret[0] = 0;
	return (0);
}
