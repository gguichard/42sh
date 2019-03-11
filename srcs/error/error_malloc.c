#include "../../includes/shell.h"
#include "../../includes/error.h"

void	ft_exit_malloc(void)
{
	write(2, "21sh: malloc failed\n", 20);
	exit(EXIT_FAILURE);
}
