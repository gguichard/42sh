#include "shell.h"

void	read_lst(t_ast *lst, int active)
{
	t_ast	*tmp;
	int		x;
	int		i;

	i = 0;
	tmp = lst;
	if (active == 0)
		return ;
	while (tmp)
	{
		ft_printf("Elem %d ___ type: %d\n", i, tmp->type);
		x = 0;
		while (tmp->input[x])
		{
			ft_printf("tmp->input[%d]: %s\n", x, tmp->input[x]);
			x += 1;
		}
		ft_printf("\n\n");
		tmp = tmp->next;
		i += 1;
	}
}

void	read_sort_descent(t_ast *sort, int active)
{
	t_ast	*tmp;
	int		i;

	tmp = sort;
	if (active == 0)
		return ;
	while (tmp->left)
	{
		ft_printf("\ntype= %d\n", tmp->type);
		i = 0;
		while (tmp->input[i])
		{
			ft_printf("input[%d]: %s\n", i, tmp->input[i]);
			i += 1;
		}
		tmp->print = 1;
		tmp = tmp->left;
	}
	ft_printf("\ntype= %d\n", tmp->type);
	i = 0;
	while (tmp->input[i])
	{
		ft_printf("input[%d]: %s\n", i, tmp->input[i]);
		i += 1;
	}
	tmp->print = 1;
	while (tmp)
	{
		if (tmp->left && tmp->left->print == 0)
		{
			tmp = tmp->left;
			ft_printf("\ntype= %d\n", tmp->type);
			i = 0;
			while (tmp->input[i])
			{
				ft_printf("input[%d]: %s\n", i, tmp->input[i]);
				i += 1;
			}
			tmp->print = 1;
		}
		else if (tmp->right && tmp->right->print == 0)
		{
			tmp = tmp->right;
			ft_printf("\ntype= %d\n", tmp->type);
			i = 0;
			while (tmp->input[i])
			{
				ft_printf("input[%d]: %s\n", i, tmp->input[i]);
				i += 1;
			}
			tmp->print = 1;
		}
		else
			tmp = tmp->back;
	}
	ft_printf("\n");
}

void	reinit_print(t_ast *lst, int active)
{
	t_ast	*tmp;

	tmp = lst;
	if (active == 0)
		return ;
	while (tmp)
	{
		tmp->print = 0;
		tmp = tmp->next;
	}
}
