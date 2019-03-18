#include "shell.h"
#include "builtins.h"

int	fg_builtins(t_ast *elem, t_alloc *alloc)
{
	(void)elem;
	if (g_pid > 0)
	{
		kill(g_pid, SIGCONT);
		waitpid(g_pid, &alloc->ret_val, WUNTRACED);
	}
	return (0);
}
