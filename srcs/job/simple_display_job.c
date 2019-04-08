/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_display_job.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/08 13:28:45 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/08 13:28:46 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "job.h"

void	display_simple_job(t_list *tmp, int index)
{
	char	current;
	t_job	*job;
	char	*state;
	char	*cmd;

	job = tmp->content;
	current = ' ';
	if (!tmp->next)
		current = '+';
	else if (!tmp->next->next)
		current = '-';
	state = job_state_str(tmp);
	cmd = job_cmd(job);
	if (job->pipe && ft_strequ("SIGTSTP", signal_stop_str(tmp)))
		ft_printf("[%d] %c %s %s\n", index, current, state, cmd);
	else if (check_job_state(tmp, STOPPED))
		ft_printf("[%d] %c %s(%s) %s\n"
			, index, current, state, signal_stop_str(tmp), cmd);
	else if (ft_strequ("Exit", state))
		ft_printf("[%d] %c %s(%d) %s\n"
			, index, current, state, last_job(job)->status, cmd);
	else
		ft_printf("[%d] %c %s %s\n", index, current, state, cmd);
	ft_memdel((void **)&cmd);
}
