#include <stdlib.h>
#include "libft.h"
#include "token_inf.h"

void			del_token(void *content, size_t content_size)
{
	(void)content_size;
	free(((t_token_inf*)content)->token);
	free(content);
}

t_token_inf		*get_tk(t_list *lst)
{
	if (lst != NULL)
		return ((t_token_inf*)(lst->content));
	else
		return (NULL);
}
