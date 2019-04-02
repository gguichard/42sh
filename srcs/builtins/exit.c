#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "shell.h"
#include "job.h"
#include "builtins.h"
#include "signals.h"

int			retry_exit_job(int type)
{
	static int	retry_job;

	if (type == -1)
		return (retry_job);
	retry_job = type;
	return (0);
}

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

static void	parkour_ast_for_exit(t_ast *elem, int *del)
{
	if (!elem || elem->type == AST_PIPE)
		return ;
	if (elem->left)
		check_exit_cmd(elem->left);
	if (elem->right)
		check_exit_cmd(elem->right);
	if (!ft_strcmp(elem->input[0], "exit"))
		*del = 0;
}

void		check_exit_cmd(t_ast *elem)
{
	int		del;

	del = 1;
	parkour_ast_for_exit(elem, &del);
	if (del)
		retry_exit_job(0);
}

int			errors_exit(t_ast *elem, int *status)
{
	char	*endptr;

	if (elem != NULL && elem->input[1] != NULL)
	{
		*status = ft_strtol(elem->input[1], &endptr, 10);
		if (*endptr != '\0')
		{
			*status = 2;
			ft_dprintf(STDERR_FILENO, "42sh: exit: %s: "
					"numeric argument required\n", elem->input[1]);
		}
	}
	if (elem && elem->back && (elem->back->type == AST_PIPE
		|| (elem->back->back && elem->back->back->type == AST_PIPE)))
		exit(*status);
	if (check_stopped_job() && !retry_exit_job(-1))
	{
		ft_dprintf(STDERR_FILENO, "There are stopped jobs.\n");
		retry_exit_job(1);
		return (1);
	}
	return (0);
}

int			builtin_exit(t_ast *elem, t_alloc *alloc)
{
	int		status;

	status = alloc->ret_val;
	if (!g_sig || g_sig == 15)
		ft_putstr("exit\n");
	if (!g_sig)
	{
		if (errors_exit(elem, &status))
			return (1);
	}
	sig_reset();
	if (!elem)
		dup2(g_cmdline->stdin_dup, STDIN_FILENO);
	terminate_all_jobs(SIGTERM);
	save_history_entries(alloc, &alloc->cmdline.history);
	reset_term(&alloc->cmdline);
	del_alloc(alloc);
	if (g_sig && g_sig != 15)
		kill(0, g_sig);
	exit(status);
	return (1);
}
