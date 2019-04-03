#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "libft.h"
#include "shell.h"
#include "job.h"
#include "builtins.h"
#include "signals.h"

int			check_stopped_job(void)
{
	t_list	*tmp;

	tmp = g_jobs;
	while (tmp)
	{
		if (check_job_state(tmp, STOPPED))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

static int	errors_arg_exit(t_ast *elem, int *status)
{
	char	*endptr;

	if (elem != NULL && elem->input[1] != NULL && elem->input[2] != NULL)
	{
		ft_putstr_fd("42sh: exit: too many arguments\n", STDERR_FILENO);
		return (1);
	}
	else if (elem != NULL && elem->input[1] != NULL)
	{
		*status = ft_strtol(elem->input[1], &endptr, 10);
		if (*endptr != '\0')
		{
			*status = 2;
			ft_dprintf(STDERR_FILENO, "42sh: exit: %s: "
					"numeric argument required\n", elem->input[1]);
		}
	}
	return (0);
}

static int	errors_exit(t_ast *elem, t_alloc *alloc, int *status)
{
	if (alloc->ppid != getpid())
	{
		errors_arg_exit(elem, status);
		exit(*status);
	}
	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (errors_arg_exit(elem, status))
		return (1);
	if (check_stopped_job() && !alloc->exit_rdy)
	{
		ft_putstr_fd("There are stopped jobs.\n", STDERR_FILENO);
		alloc->exit_rdy = 1;
		return (1);
	}
	return (0);
}

int			builtin_exit(t_ast *elem, t_alloc *alloc)
{
	int		status;

	status = alloc->ret_val;
	if (!g_sig)
	{
		if (errors_exit(elem, alloc, &status))
			return (1);
	}
	else if (g_sig == 15)
		ft_putstr_fd("exit\n", STDERR_FILENO);
	sig_reset();
	terminate_all_jobs(SIGTERM);
	save_history_entries(alloc, &alloc->cmdline.history);
	if (elem == NULL)
	{
		dup2(g_cmdline->stdin_dup, STDIN_FILENO);
		reset_term(&alloc->cmdline);
	}
	del_alloc(alloc);
	if (g_sig && g_sig != 15)
		kill(0, g_sig);
	exit(status);
	return (1);
}
