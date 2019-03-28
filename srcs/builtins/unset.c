#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "vars.h"
#include "builtins.h"
#include "hashtable.h"

static int	unset_builtin_var(t_alloc *alloc, const char *key)
{
	if (!is_var_valid_identifier(key))
		return (0);
	unset_var(&alloc->vars, key);
	if (ft_strequ(key, "PATH"))
		delete_hashentries(alloc->exectable);
	return (1);
}

int			builtin_unset(t_ast *elem, t_alloc *alloc)
{
	int	idx;
	int	ret;

	idx = 1;
	ret = 0;
	while (elem->input[idx] != NULL)
	{
		if (!unset_builtin_var(alloc, elem->input[idx]))
		{
			ft_dprintf(2, "42sh: unset: '%s': not a valid identifier"
					, elem->input[idx]);
			ret = 1;
		}
		idx++;
	}
	return (ret);
}
