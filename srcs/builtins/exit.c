/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:27:19 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/09 17:45:33 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "libft.h"
#include "shell.h"
#include "job.h"
#include "signals.h"
#include "builtins.h"

int			check_stopped_job(void)
{
	t_list	*tmp;

	tmp = g_jobs;
	sleep(5);
	while (tmp != NULL)
	{
		if (check_job_state(tmp, STOPPED))
			return (1);
		tmp = tmp->next;
	}
	return (0);
}

static int	check_numeric_arg(const char *arg, int *status)
{
	char	*endptr;

	*status = ft_strtol(arg, &endptr, 10);
	if (*endptr != '\0')
	{
		ft_dprintf(STDERR_FILENO, "42sh: exit: %s: "
				"numeric argument required\n", arg);
		return (0);
	}
	return (1);
}

static void	clean_everything_before_exit(t_alloc *alloc, int is_from_cmdline)
{
	if (alloc->ppid == getpid() && alloc->is_interactive)
		ft_dprintf(STDERR_FILENO, "exit\n");
	save_history_entries(alloc, &alloc->cmdline.history);
	terminate_all_jobs(g_sig == SIGHUP ? SIGHUP : SIGTERM);
	if (is_from_cmdline)
	{
		dup2(alloc->cmdline.stdin_dup, STDIN_FILENO);
		close(alloc->cmdline.stdin_dup);
		reset_term(&alloc->cmdline);
	}
	sig_reset();
	del_alloc(alloc);
	if (g_sig && g_sig != 15)
		kill(0, g_sig);
}

int			builtin_exit(t_ast *elem, t_alloc *alloc)
{
	int	status;

	status = alloc->ret_val;
	if (elem != NULL && ft_strtab_count(elem->input) > 2)
		ft_dprintf(STDERR_FILENO, "42sh: exit: too many arguments\n");
	else
	{
		if (elem != NULL && elem->input[1] != NULL
				&& !check_numeric_arg(elem->input[1], &status))
			return (2);
		else if (check_stopped_job() && !alloc->exit_rdy)
		{
			ft_putstr_fd("There are stopped jobs.\n", STDERR_FILENO);
			alloc->exit_rdy = 1;
			return (1);
		}
		clean_everything_before_exit(alloc, elem == NULL);
		exit(status);
	}
	return (1);
}
