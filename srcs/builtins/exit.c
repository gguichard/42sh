#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "shell.h"
#include "job.h"
#include "builtins.h"

static int	retry_exit_job(int type)
{
	static int	retry_job;

	if (type == -1)
		return (retry_job);
	retry_job = type;
	return (0);
}

static int	check_stopped_job(void)
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
	if (!elem || elem->type == OPERATOR)
		return ;
	if (elem->left)
		check_exit_cmd(elem->left);
	if (elem->right)
		check_exit_cmd(elem->right);
	if (!ft_strcmp(elem->input[0], "exit"))
		del = 0;
}

void		check_exit_cmd(t_ast *elem)
{
	int		del;

	del = 1;
	parkour_ast_for_exit(elem, &del);
	if (del)
		retry_exit_job(0);
}

int			builtin_exit(t_ast *elem, t_alloc *alloc)
{
	int			status;
	char		*endptr;

	status = alloc->ret_val;
	if (check_stopped_job() && !retry_exit_job(-1))
	{
		ft_dprintf(STDERR_FILENO, "There are stopped jobs.\n");
		retry_exit_job(1);
		return (1);
	}
	if (elem && elem->input[1] != NULL)
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
	terminate_all_jobs();
	ft_putstr("exit\n");
	exit(status);
	return (1);
}
