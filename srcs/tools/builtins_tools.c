#include "shell.h"
#include "builtins.h"

int	is_builtin(t_alloc *alloc, const char *built_name)
{
	int	idx;

	idx = 0;
	while (alloc->builtins[idx].name)
	{
		if (ft_strequ(alloc->builtins[idx].name, built_name))
			return (1);
		idx += 1;
	}
	return (0);
}
