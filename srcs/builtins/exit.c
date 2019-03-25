#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "shell.h"
#include "builtins.h"

int	builtin_exit(t_ast *elem, t_alloc *alloc)
{
	int		status;
	char	*endptr;

	status = 0; // TODO: last exit status
	if (elem->input[1] != NULL)
	{
		status = ft_strtol(elem->input[1], &endptr, 10);
		if (*endptr != '\0')
		{
			status = 2;
			ft_dprintf(STDERR_FILENO, "42sh: exit: %s: "
					"numeric argument required\n", elem->input[1]);
		}
	}
	del_alloc(alloc);
	ft_putstr("exit\n");
	exit(status);
	return (1);
}
