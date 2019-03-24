/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 10:18:39 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/24 23:22:51 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "history.h"

t_history_entry	*create_history_entry(const char *content)
{
	t_history_entry	*entry;

	if (content == NULL || content[0] == '\0')
		return (NULL);
	entry = (t_history_entry *)malloc(sizeof(t_history_entry));
	if (entry == NULL)
		return (NULL);
	entry->content = ft_strdup(content);
	if (entry->content == NULL)
	{
		free(entry);
		return (NULL);
	}
	return (entry);
}

void			add_history_entry(t_history *history, const char *content)
{
	t_history_entry	*entry;

	entry = create_history_entry(content);
	if (entry != NULL)
	{
		entry->next = history->front;
		entry->prev = NULL;
		if (entry->next != NULL)
			entry->next->prev = entry;
		if (history->back == NULL)
			history->back = entry;
		history->front = entry;
	}
}

void			push_history_entry(t_history *history, const char *content)
{
	t_history_entry	*entry;

	entry = create_history_entry(content);
	if (entry != NULL)
	{
		entry->next = NULL;
		entry->prev = history->back;
		if (entry->prev != NULL)
			entry->prev->next = entry;
		if (history->front == NULL)
			history->front = entry;
		history->back = entry;
	}
}

const char		*peek_history_prev(t_history *history)
{
	if (history->offset == NULL)
		history->offset = history->back;
	else
		history->offset = history->offset->prev;
	if (history->offset == NULL)
	{
		history->offset = history->front;
		return (NULL);
	}
	return (history->offset->content);
}

const char		*peek_history_next(t_history *history)
{
	if (history->offset == NULL)
		return (NULL);
	history->offset = history->offset->next;
	if (history->offset == NULL)
		return (NULL);
	return (history->offset->content);
}
