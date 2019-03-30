#include "shell.h"

char	*ft_tab_to_str(char **tab)
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

void	read_sort_descent(t_ast *sort, int active)
{
	t_ast	*tmp;
	char	*s;

	tmp = sort;
	ft_printf("\n");
	if (!tmp)
		return ;
	active += 10;
	read_sort_descent(tmp->right, active);
	ft_printf("\n");
	for (int i = 10; i < active; i++) {
		ft_printf(" ");
	}
	s = ft_tab_to_str(tmp->input);
	ft_printf("input= %s\n", s);
	ft_memdel((void **)&s);
	read_sort_descent(tmp->left, active);
}
