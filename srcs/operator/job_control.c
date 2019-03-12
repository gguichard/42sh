#include "shell.h"
#include "operator.h"

int	job_control(t_ast *elem, t_var *lst_env)
{
	(void)elem;
	(void)lst_env;
	write(2, "21sh: no job control\n", 21);
	return (1);
}
