#include "libft.h"
#include "shell.h"
#include "builtins.h"

// BUILTINS SET POSIX NORME BUT NO OPTIONS
// JUST DISPLAY ALL VARIABLES
// IF ARGUMENTS RETURN ERROR WITH USAGE

int	builtin_set(t_ast *elem, t_alloc *alloc)
{
	t_var	*tmp;

	tmp = *(alloc->var);
	if (elem->input[1])
	{
		ft_dprintf(2, "42sh: set: %s: invalid usage\nset: usage: set"
				, elem->input[1]);
		return (1);
	}
	while (tmp)
	{
		if (tmp->value)
			ft_printf("%s=%s\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	return (0);
}
