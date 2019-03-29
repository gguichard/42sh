#include <stdlib.h>
#include <unistd.h>
#include "error.h"

void	ft_exit_malloc(void)
{
	ft_dprintf(STDERR_FILENO, "42sh: malloc failed\n");
	exit(EXIT_FAILURE);
}
