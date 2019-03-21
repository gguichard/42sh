/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 10:18:39 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/21 11:18:36 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"
#include "history.h"

void		add_history_entry(t_history *history, const char *cmdline)
{
	t_history_entry	*entry;

	entry = (t_history_entry *)malloc(sizeof(t_history_entry));
	if (entry == NULL)
		return ;
	entry->content = ft_strdup(cmdline);
	if (entry->content == NULL)
	{
		free(entry);
		return ;
	}
	entry->next = NULL;
	entry->prev = history->back;
	if (entry->prev != NULL)
		entry->prev->next = entry;
	if (history->front == NULL)
		history->front = entry;
	history->back = entry;
	history->offset = NULL;
}

const char	*peek_history_prev(t_history *history)
{
	if (history->offset != NULL)
		history->offset = history->offset->prev;
	else
		history->offset = history->back;
	if (history->offset == NULL)
		return (NULL);
	return (history->offset->content);
}

const char	*peek_history_next(t_history *history)
{
	if (history->offset != NULL)
		history->offset = history->offset->next;
	if (history->offset == NULL)
		return (NULL);
	return (history->offset->content);
}
