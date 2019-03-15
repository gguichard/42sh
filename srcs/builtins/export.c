#include "shell.h"
#include "builtins.h"
#include "error.h"

static int	check_options(t_ast *elem, int *i)
{
	int	x;

	x = 0;
	while (elem->input[*i] && elem->input[*i][x] == '-')
	{
		x = 1;
		while (elem->input[*i][x] == 'p')
			x += 1;
		if (elem->input[*i][x])
			return (error_export(elem->input[*i][x]));
		x = 1;
		*i += 1;
	}
	return (0);
}

static void	display_lst_var(t_var *lst)
{
	while (lst)
	{
		if (lst->is_env == 1)
			ft_printf("declare -x %s=\"%s\"\n", lst->key, lst->value);
		lst = lst->next;
	}
}

// static void	add_new_var(t_var **lst, char *var)
// {
//
// }

static void	export_var(t_var **lst, t_ast *elem, int i)
{
	t_var	*tmp;

	(void)lst;
	tmp = NULL;
	while (elem->input[i])
	{
		// if (!(tmp = find_elem_env(*lst, key)))
		// 	add_new_var(lst, elem->input[i]);
		// else
		// {
			// ft_memdel((void **)&(tmp->value));
			// if (!(tmp->value = ft_strdup(value)))
				// ft_exit_malloc();
		// }
		i += 1;
	}
}

int	export_builtins(t_ast *elem, t_alloc *alloc)
{
	int	i;
	int	ret;

	(void)alloc;
	i = 1;
	ret = 0;
	if ((ret = check_options(elem, &i)) == 1)
		return (ret);
	if (i != 1)
		display_lst_var(*(alloc->var));
	else
		export_var(alloc->var, elem, i);
	return (0);
}
