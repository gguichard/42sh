#include "shell.h"
#include "parser_lexer.h"
#include "error.h"

t_ast	*create_new_elem(t_ast **lst)
{
	t_ast	*new;

	if (!(new = (t_ast *)malloc(sizeof(t_ast))))
		ft_exit_malloc();
	new->print = 0;
	new->type = NO_TYPE;
	new->input = NULL;
	new->heredoc = NULL;
	new->next = NULL;
	new->back = NULL;
	new->left = NULL;
	new->right = NULL;
	(*lst == NULL) ? *lst = new : 0;
	return (new);
}

t_ast	*get_last_elem(t_ast *lst)
{
	t_ast *elem;

	elem = lst;
	while (elem->next != NULL)
		elem = elem->next;
	return (elem);
}

t_ast	*add_new_elem(t_ast **lst)
{
	t_ast	*end;
	t_ast	*new;

	new = create_new_elem(lst);
	end = get_last_elem(*lst);
	end->next = new;
	new->back = end;
	return (new);
}

void	ft_custom_memmove(char *dst, char *src, size_t len)
{
	size_t	i;

	i = 0;
	if (src == dst)
		return ;
	if (src < dst)
	{
		while ((int)len > 0)
		{
			dst[len] = '\0';
			dst[len - 1] = src[len - 1];
			len -= 1;
		}
	}
	else
	{
		while (i < len)
		{
			dst[i] = src[i];
			i += 1;
		}
		dst[i] = '\0';
	}
}
