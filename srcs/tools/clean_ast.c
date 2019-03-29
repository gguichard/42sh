#include <stdlib.h>
#include "libft.h"
#include "shell.h"

void	del_elem_ast(t_ast **lst)
{
	if (lst == NULL || *lst == NULL)
		return ;
	ft_memdel((void **)((*lst)->heredoc));
	delete_str_tab((*lst)->input);
	ft_memdel((void **)lst);
}

void	del_ast(t_ast **lst)
{
	if (lst == NULL || *lst == NULL)
		return ;
	if ((*lst)->left != NULL)
		del_ast(&(*lst)->left);
	if ((*lst)->right != NULL)
		del_ast(&(*lst)->right);
	del_elem_ast(lst);
}
