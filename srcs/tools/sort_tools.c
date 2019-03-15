#include "shell.h"

void	insert_new_elem(t_var **lst, t_var *new)
{
	t_var *tmp;

	tmp = *lst;
	if (!lst || !(tmp))
		*lst = new;
	else if (ft_strcmp(new->key, tmp->key) <= 0)
	{
		new->next = tmp;
		*lst = new;
	}
	else
	{
		while (tmp && tmp->next && ft_strcmp(new->key, tmp->next->key) > 0)
			tmp = tmp->next;
		new->next = tmp->next;
		tmp->next = new;
	}
}
