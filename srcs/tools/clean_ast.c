#include "shell.h"

void	del_elem_ast(t_ast **lst)
{
	if (!lst || !(*lst))
		return ;
	ft_memdel((void **)((*lst)->heredoc));
	delete_str_tab((*lst)->input);
	free((*lst));
	*lst = NULL;
}

void	del_ast(t_ast **lst)
{
	if (!lst || !(*lst))
		return ;
	if ((*lst)->left)
		del_ast(&((*lst)->left));
	if ((*lst)->right)
		del_ast(&((*lst)->right));
	del_elem_ast(lst);
	return ;
}
