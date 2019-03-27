#include <unistd.h>
#include "libft.h"
#include <unistd.h>
#include "shell.h"
#include "vars.h"
#include "builtins.h"

// BUILTINS SET POSIX NORME BUT NO OPTIONS
// JUST DISPLAY ALL VARIABLES
// IF ARGUMENTS RETURN ERROR WITH USAGE

int	builtin_set(t_ast *elem, t_alloc *alloc)
{
	t_list	*cur;
	t_var	*var;

	if (elem->input[1] != NULL)
	{
		ft_dprintf(STDERR_FILENO, "42sh: set: %s: invalid usage\n"
				"set: usage: set\n", elem->input[1]);
		return (1);
	}
	cur = alloc->vars;
	while (cur != NULL)
	{
		var = (t_var *)cur->content;
		if (var->value != NULL)
			ft_printf("%s=%s\n", var->key, var->value);
		cur = cur->next;
	}
	return (0);
}
