#include "shell.h"
#include "error.h"

int	error_export(char c)
{
	ft_dprintf(2, "42sh: export: -%c: invalid option\
	\nexport: usage: [name[=value] ...] or export -p\n", c);
	return (1);
}
