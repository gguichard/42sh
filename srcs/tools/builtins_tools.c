#include "shell.h"
#include "builtins.h"

int	is_builtins(t_alloc *alloc, const char *built_name)
{
	int	i;

	i = 0;
	while (alloc->builtins[i].name)
	{
		if (ft_strequ(alloc->builtins[i].name, built_name) == 0)
			return (1);
		i += 1;
	}
	return (0);
}
