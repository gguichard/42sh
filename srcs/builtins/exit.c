#include "../../includes/shell.h"
#include "../../includes/builtins.h"

int	exit_builtins(t_ast *elem, t_alloc *alloc)
{
	(void)alloc;
	if (!elem->input[1])
		exit(0);
	exit(ft_atoi(elem->input[1]));
	return (0);
}
