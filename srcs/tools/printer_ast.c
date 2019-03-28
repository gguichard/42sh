#include "shell.h"

char		*ft_tab_to_str(char **tab)
{
	size_t	x;
	size_t	y;
	char	*out;

	if (!tab)
		return (0);
	y = 0;
	x = 0;
	while (tab[y])
		x += ft_strlen(tab[y++]) + 1;
	if (!(out = ft_memalloc(x + 1)))
		return (0);
	y = 0;
	while (tab[y])
		ft_strcat(ft_strcat(out, tab[y++]), " ");
	return (out);
}

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
	// char	*s;
	int		i;

	(void)active;

	tmp = sort;
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
	// if (!tmp)
	// 	return ;
	// active += 10;
	// read_sort_descent(tmp->right, active);
	// ft_printf("\n");
	// for (i = 10; i < active; i++) {
	// 	ft_printf(" ");
	// }
	// s = ft_tab_to_str(tmp->input);
	// ft_printf("input= %s\n", s);
	// ft_memdel((void **)&s);
	// read_sort_descent(tmp->left, active);
	return ;
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
