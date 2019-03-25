#include "libft.h"
#include "shell.h"
#include "vars.h"

void	del_lst_ast(t_ast **lst)
{
	t_ast	*tmp;
	int		i;

	i = 0;
	tmp = *lst;
	if (!lst || !tmp)
		return ;
	while (tmp)
	{
		*lst = tmp->next;
		ft_memdel((void **)(tmp->heredoc));
		ft_strtab_free(tmp->input);
		free(tmp);
		tmp = *lst;
	}
	*lst = NULL;
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
