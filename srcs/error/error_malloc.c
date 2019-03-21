#include "shell.h"
#include "error.h"

void	ft_exit_malloc(void)
{
	write(2, "42sh: malloc failed\n", 20);
	exit(EXIT_FAILURE);
}
