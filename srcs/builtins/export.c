#include "shell.h"
#include "builtins.h"

static int	check_options(t_ast *elem, int *i)
{
	int	x;

	x = 1;
	while (elem->input[*i] && elem->input[*i][x] == '-')
	{
		while (elem->input[*i][x] == 'p')
			x += 1;
		if (elem->input[*i][x])
			return (error_export(elem->input[*i][x]));
		x = 1;
		*i += 1;
	}
	return (0);
}

static void	display_lst_env(t_var **lst)
{
	t_var *tmp;

	tmp = *lst;
	while (tmp)
	{
		if (tmp->is_env == 1)
			ft_printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
}

static int	elem_env_exist(t_var **lst, char *name)
{
	
}

static void	add_elem_env(t_var **lst, t_ast *elem, int i)
{
	while (elem->input[i])
}

int	export_builtins(t_ast *elem, t_var *lst_env, t_alloc *alloc)
{
	int	i;
	int	ret;

	(void)alloc;
	i = 1;
	ret = 0;
	if ((ret = check_options(elem, &i)) == 1)
		return (ret);
	if (i != 1)
		display_lst_env(&lst_env);
	else
		add_elem_env(&lst_env, elem, i);
	return (0);
}
