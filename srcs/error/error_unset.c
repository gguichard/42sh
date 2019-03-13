#include "shell.h"
#include "error.h"

int	error_unset(char *str)
{
	ft_dprintf(2, "42sh: unset: %s: not a valid identifier", str);
	return (1);
}
