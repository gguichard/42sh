#include "shell.h"
#include "operator.h"
#include "builtins.h"
#include "parser_lexer.h"

static void		add_cp_var(t_var **tmp, char *key, char *value, int is_env)
{
	t_var	*new;

	if (!(new = (t_var*)malloc(sizeof(t_var))))
		ft_exit_malloc();
	if (!(new->key = ft_strdup(key)))
		ft_exit_malloc();
	if (!(new->value = ft_strdup(value)))
		ft_exit_malloc();
	new->next = NULL;
	new->is_env = is_env;
	insert_new_elem(tmp, new);
}

static t_var	*create_tmp_var_lst(t_ast *elem, t_var *lst)
{
	t_var	*tmp;
	int		i;
	int		x;

	tmp = NULL;
	i = 0;
	x = 0;
	while (lst)
	{
		add_cp_var(&tmp, lst->key, lst->value, lst->is_env);
		lst = lst->next;
	}
	while (elem->input[i])
	{
		x = 0;
		while (elem->input[i][x] != '=')
			x += 1;
		add_var(&tmp, elem->input[i], x, 0);
		i += 1;
	}
	return (tmp);
}

int	dispatch_assign(t_ast *elem, t_alloc *alloc, int no_fork)
{
	t_var	*tmp;
	t_var	**save;
	int		i;
	int		x;

	i = 0;
	save = alloc->var;
	if (elem->left && elem->left->type == CMD)
	{
		tmp = create_tmp_var_lst(elem, *(alloc->var));
		alloc->var = &tmp;
		analyzer(elem, alloc, no_fork);
		del_lst_env(&tmp);
		alloc->var = save;
	}
	else
		while (elem->input[i])
		{
			x = 0;
			while (elem->input[i][x] != '=')
				x += 1;
			add_var(alloc->var, elem->input[i], x, 0);
			i += 1;
		}
	return (0);
}
