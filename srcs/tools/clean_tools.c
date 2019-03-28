#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "vars.h"

void	del_lst_ast(t_ast **lst)
{
	t_ast	*tmp;

	tmp = *lst;
	if (lst == NULL || tmp == NULL)
		return ;
	*lst = (*lst)->back;
	ft_strdel(&tmp->heredoc);
	ft_strtab_free(tmp->input);
	free(tmp);
}

void	del_alloc(t_alloc *alloc)
{
	if (alloc == NULL)
		return ;
	if (alloc->vars != NULL)
		ft_lstdel(&alloc->vars, free_var);
	if (alloc->ast)
	{
		del_lst_ast(&alloc->ast);
		alloc->ast = NULL;
	}
	alloc = NULL;
}
