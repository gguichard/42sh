#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "vars.h"
#include "builtins.h"

// BUILTINS UNSET POSIX NORME NO OPTIONS
// JUST UNSET ALL THE VARIABLES
// IF THERE IS AN '=' IN THE NAME VARIABLE
// RETURN ERROR

int	builtin_unset(t_ast *elem, t_alloc *alloc)
{
	int	idx;
	int	ret;

	idx = 1;
	ret = 0;
	while (elem->input[idx] != NULL)
	{
		if (is_var_valid_identifier(elem->input[idx]))
			unset_var(&alloc->vars, elem->input[idx]);
		else
		{
			ft_dprintf(2, "42sh: unset: '%s': not a valid identifier"
					, elem->input[idx]);
			ret = 1;
		}
		idx++;
	}
	return (ret);
}
