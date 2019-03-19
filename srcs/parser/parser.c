#include "shell.h"
#include "parser_lexer.h"
#include "error.h"
#include "token_inf.h"


/*
** TODO:
**	 - Check coherence token
**	 - create ast:
**			- get length input
**			- fill INPUT
**			- Add type
**			- insertion sort new elem
*/





static t_ast	*create_ast(t_list *lst_tk)
{
	t_list	*tmp;
	t_ast	*ast;

	tmp = lst_tk;
	ast = create_new_elem(&ast);
	while (tmp)
	{
		tmp = tmp->next;
	}
}

t_ast			*parser(t_list *lst_tk, t_alloc *alloc)
{
	t_ast	*sort;
	int		j;

	(void)alloc;
	sort = NULL;
	j = 0;

	//ADD FUNCTION TO CHECK IF CMD IS VALID

	sort = create_ast(lst_tk);
	// while (lst_tk)
	// {
	// 	ft_printf("type[%d]: %d\nvalue[%d]: |%s|\n\n", j, get_tk(lst_tk)->type, j, get_tk(lst_tk)->token);
	// 	lst_tk = lst_tk->next;
	// 	j += 1;
	// }
	return (sort);
}
