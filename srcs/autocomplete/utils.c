#include <stdlib.h>
#include "libft.h"
#include "autocomplete.h"

size_t			count_same_char(const char *str1, const char *str2)
{
	int	count;

	count = 0;
	while (str1[count] != '\0' && str1[count] == str2[count])
		++count;
	return (count);
}

static void		add_to_lst_here(t_list **lst, t_list *elem, t_list *prev
		, t_list *curr)
{
	if (prev == NULL)
	{
		elem->next = *lst;
		*lst = elem;
	}
	else
	{
		elem->next = curr;
		prev->next = elem;
	}
}

int				strlist_insert_sort(t_list **lst, t_list *elem)
{
	t_list	*prev;
	t_list	*curr;
	int		cmp_res;

	prev = NULL;
	curr = *lst;
	while (curr != NULL)
	{
		cmp_res = ft_strcmp((char *)elem->content, (char *)curr->content);
		if (cmp_res == 0)
		{
			free(elem->content);
			free(elem);
			return (0);
		}
		else if (cmp_res < 0)
			break ;
		prev = curr;
		curr = curr->next;
	}
	add_to_lst_here(lst, elem, prev, curr);
	return (1);
}
