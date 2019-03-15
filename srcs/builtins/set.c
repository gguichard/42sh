#include "shell.h"
#include "builtins.h"
#include "error.h"

// BUILTINS SET POSIX NORME BUT NO OPTIONS
// JUST DISPLAY ALL VARIABLES
// IF ARGUMENTS RETURN ERROR WITH USAGE

int	set_builtins(t_ast *elem, t_alloc *alloc)
{
	t_var	*tmp;

	(void)alloc;
	tmp = *(alloc->var);
	if (elem->input[1])
		return (error_set(elem->input[1]));
	while (tmp)
	{
		ft_printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}
