/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_events.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/26 14:35:05 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/26 16:13:58 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "history.h"

static t_history_entry	*search_history_entry(t_history *history
		, const char *start)
{
	size_t			start_len;
	t_history_entry	*cur;

	start_len = ft_strlen(start);
	cur = history->back;
	while (cur != NULL)
	{
		if (ft_strnequ(cur->content, start, start_len))
			return (cur);
		cur = cur->prev;
	}
	return (NULL);
}

static t_history_entry	*get_history_entry(t_history *history, long number)
{
	int				backward;
	t_history_entry	*cur;

	backward = 0;
	if (number < 0)
	{
		number = -number;
		cur = history->back;
	}
	else
	{
		cur = history->front;
		backward = 1;
	}
	while (cur != NULL && --number > 0)
	{
		if (backward)
			cur = cur->next;
		else
			cur = cur->prev;
	}
	return (cur);
}

static t_history_entry	*replace_misc_event(t_history *history, char *event)
{
	t_history_entry	*entry;
	char			*endptr;
	long			number;

	if (event[1] == '!')
	{
		entry = history->back;
		endptr = event + 2;
	}
	else if (!ft_strchr("-+", event[1]) && !ft_isdigit(event[1]))
	{
		entry = search_history_entry(history, endptr);
		endptr = event + ft_strlen(event);
	}
	else
	{
		number = ft_strtol(event + 1, &endptr, 10);
		if (number == 0)
			return (NULL);
		entry = get_history_entry(history, number);
	}
	if (entry != NULL)
		*endptr = '\0';
	return (entry);
}

int						replace_event(t_history *history, char **input
		, size_t *offset, char *event)
{
	t_history_entry	*entry;

	entry = replace_misc_event(history, event);
	if (entry != NULL)
	{
		if (ft_strreplace_inside(input, *offset, ft_strlen(event)
					, entry->content))
		{
			*offset += (ft_strlen(entry->content) - 1);
			return (1);
		}
	}
	return (0);
}
