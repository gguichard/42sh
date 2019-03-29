#include "shell.h"
#include "operator.h"
#include "builtins.h"
#include "execution.h"
#include "vars.h"

static int		add_cp_var(t_list **tmp, char *key, char *value, int is_env)
{
	t_var	var;
	t_list	*elem;

	var.key = ft_strdup(key);
	var.value = ft_strdup(value);
	var.is_env = is_env;
	if (var.key != NULL && var.value != NULL)
	{
		elem = ft_lstnew(&var, sizeof(t_var));
		if (elem != NULL)
		{
			insert_new_elem(tmp, elem);
			return (1);
		}
	}
	free(var.key);
	free(var.value);
	return (0);
}

static void		add_var(t_list **tmp, char *input, int i, int is_env)
{
	t_var	var;
	t_var	*new;
	t_list	*elem;
	size_t	len;

	len = ft_strlen(input);
	var.key = ft_strsub(input, 0, i);
	var.is_env = is_env;
	if ((size_t)i != len)
		var.value = ft_strsub(input, i + 1, len - i - 1);
	else
		var.value = NULL;
	if ((new = get_var(*tmp, var.key)))
	{
		ft_memdel((void **)&(new->value));
		new->value = var.value;
		ft_memdel((void **)&(var.key));
		return ;
	}
	elem = ft_lstnew(&var, sizeof(t_var));
	if (elem != NULL)
		insert_new_elem(tmp, elem);
}

static t_list	*create_tmp_var_lst(t_ast *elem, t_list *lst)
{
	t_list	*tmp;
	int		i;
	int		x;

	tmp = NULL;
	i = 0;
	x = 0;
	while (lst)
	{
		add_cp_var(&tmp, ((t_var *)lst->content)->key,
			((t_var *)lst->content)->value, ((t_var *)lst->content)->is_env);
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

int	dispatch_assign(t_ast *elem, t_alloc *alloc, t_exec_opt *opt)
{
	t_list	*tmp;
	t_list	*save;
	int		i;
	int		x;

	i = 0;
	save = alloc->vars;
	if (elem->left && elem->left->type == AST_CMD)
	{
		tmp = create_tmp_var_lst(elem, alloc->vars);
		alloc->vars = tmp;
		analyzer(elem, alloc, opt);
		ft_lstdel(&tmp, &free_var);
		alloc->vars = save;
	}
	else
		while (elem->input[i])
		{
			x = 0;
			while (elem->input[i][x] != '=')
				x += 1;
			add_var(&(alloc->vars), elem->input[i], x, 0);
			i += 1;
		}
	return (0);
}
