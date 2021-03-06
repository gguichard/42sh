/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   subcmd_wait_n_signal.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 14:41:21 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/10 12:19:55 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <sys/wait.h>
#include "shell.h"
#include "job.h"

static void	del_subshell_job(t_list *sub_shell)
{
	t_list	*prev;

	prev = g_jobs;
	while (prev->next != NULL && prev->next != sub_shell)
		prev = prev->next;
	if (prev == sub_shell)
		g_jobs = NULL;
	ft_memdel((void **)&(sub_shell->content));
	ft_memdel((void **)&sub_shell);
	if (g_jobs)
		prev->next = NULL;
}

static void	wait_sub_shell(pid_t child, t_alloc *alloc)
{
	t_list	*tmp;
	int		ret;

	if (!(tmp = add_pid_lst(child, 0, 0)))
		return ;
	((t_job *)tmp->content)->state = SUB_CMD;
	waitpid(child, &ret, WUNTRACED);
	if (WIFEXITED(ret))
		alloc->ret_val = WEXITSTATUS(ret);
	else if (WIFSTOPPED(ret))
		alloc->ret_val = WSTOPSIG(ret) + 128;
	else if (WIFSIGNALED(ret))
		alloc->ret_val = WTERMSIG(ret) + 128;
	del_subshell_job(tmp);
}

static void	handler_subcmd(int sig)
{
	t_list	*tmp;

	tmp = g_jobs;
	while (tmp != NULL && ((t_job *)tmp->content)->state != SUB_CMD)
		tmp = tmp->next;
	if (tmp != NULL)
		kill(((t_job *)tmp->content)->pid, sig);
	write(STDOUT_FILENO, "\n", 1);
	g_sig = sig;
}

int			sig_wait_subcmd(pid_t child, t_alloc *alloc)
{
	struct sigaction	act;

	sigfillset(&act.sa_mask);
	act.sa_handler = handler_subcmd;
	act.sa_flags = SA_RESTART;
	sigaction(SIGINT, &act, 0);
	wait_sub_shell(child, alloc);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, 0);
	return (g_sig == SIGINT);
}
