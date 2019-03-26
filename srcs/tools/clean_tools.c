#include "libft.h"
#include "shell.h"
#include "vars.h"

void	delete_str_tab(char **tab_str)
{
	int	i;

	i = 0;
	if (!tab_str)
		return ;
	while (tab_str[i])
	{
		ft_memdel((void **)&(tab_str[i]));
		i += 1;
	}
	free(tab_str);
	tab_str = NULL;
}

void	del_lst_ast(t_ast **lst)
{
	t_ast	*tmp;
	int		i;

	i = 0;
	tmp = *lst;
	if (!lst || !tmp)
		return ;
	*lst = (*lst)->back;
	ft_memdel((void **)(tmp->heredoc));
	delete_str_tab(tmp->input);
	free(tmp);
	tmp = NULL;
}

void	del_alloc(t_alloc *alloc)
{
	if (!alloc)
		return ;
	if (alloc->vars != NULL)
		ft_lstdel(&alloc->vars, free_var);
	if (alloc->ast)
	{
		del_lst_ast(&(alloc->ast));
		alloc->ast = NULL;
	}
	alloc = NULL;
}
