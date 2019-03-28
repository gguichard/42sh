#include "shell.h"
#include "operator.h"
#include "execution.h"

int	job_control(t_ast *elem, t_alloc *alloc, t_exec_opt *opt)
{
	opt->wait_hang = true;
	analyzer(elem, alloc, opt);
	return (1);
}
