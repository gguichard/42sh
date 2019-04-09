/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_id_nb.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/09 12:25:46 by jocohen           #+#    #+#             */
/*   Updated: 2019/04/09 12:30:56 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "job.h"

int	job_id_nb(t_list **tmp, int value)
{
	int	index;

	index = 1;
	while ((*tmp) && value > index)
	{
		index += 1;
		(*tmp) = (*tmp)->next;
	}
	return (value);
}

int	current_job_id(t_list **tmp)
{
	return (!(*tmp)->next);
}

int	previous_job_id(t_list **tmp)
{
	return ((!(*tmp)->next || !(*tmp)->next->next));
}

int	look_for_recent_job(t_list **tmp, int (*valid_job)(t_list **))
{
	int	index;

	index = 1;
	while (!valid_job(tmp))
	{
		index += 1;
		(*tmp) = (*tmp)->next;
	}
	return (index);
}
