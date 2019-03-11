#include "../../includes/shell.h"
#include "../../includes/builtins.h"

int	exit_builtins(t_ast *elem, t_var **lst_env, t_alloc **alloc)
{
	(void)lst_env;
	(void)alloc;
	if (!elem->input[1])
		exit(0);
	exit(ft_atoi(elem->input[1]));
	return (0);
}
