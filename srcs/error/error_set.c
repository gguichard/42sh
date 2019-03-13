#include "shell.h"
#include "error.h"

int	error_set(char *str)
{
	ft_dprintf(2, "42sh: set: %s: invalid usage\nset: usage: set", str);
	return (1);
}
