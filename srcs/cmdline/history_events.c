/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_events.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/26 14:35:05 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/26 14:59:56 by gguichar         ###   ########.fr       */
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
	t_history_entry	*cur;

	cur = history->back;
	while (cur != NULL && --number > 0)
		cur = cur->prev;
	return (cur);
}

static t_history_entry	*replace_misc_event(t_history *history, char *event)
{
	t_history_entry	*entry;
	char			*endptr;
	long			number;

	if (event[1] == '!')
	{
		if ((entry = history->back) != NULL)
			event[2] = '\0';
	}
	else if (event[1] == '-')
	{
		if ((number = ft_strtol(event + 2, &endptr, 10)) == 0)
			return (0);
		if ((entry = get_history_entry(history, number)) != NULL)
			*endptr = '\0';
	}
	else
	{
		if ((entry = search_history_entry(history, event + 1)) != NULL)
			event[ft_strlen(event)] = '\0';
	}
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
			*offset += ft_strlen(entry->content);
			return (1);
		}
	}
	return (0);
}
