#include "shell.h"
#include "operator.h"

int	job_control(t_ast *elem, t_alloc *alloc)
{
	(void)elem;
	(void)alloc;
	write(2, "21sh: no job control\n", 21);
	return (1);
}
