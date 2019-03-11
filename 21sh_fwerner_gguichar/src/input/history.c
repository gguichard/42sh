/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/09 09:54:41 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/11 09:35:48 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "shell.h"

void		clear_history_entry(t_history *elem)
{
	free(elem->content);
	free(elem);
}

void		clear_history(t_shell *shell)
{
	t_history	*curr;
	t_history	*prev;

	curr = shell->history;
	while (curr != NULL)
	{
		prev = curr->prev;
		clear_history_entry(curr);
		curr = prev;
	}
	shell->history = NULL;
	shell->history_off = NULL;
}

void		add_history_entry(t_shell *shell, const char *command)
{
	t_history	*elem;

	elem = (t_history *)malloc(sizeof(t_history));
	if (elem == NULL)
		return ;
	elem->content = ft_strdup(command);
	if (elem->content == NULL)
	{
		free(elem);
		return ;
	}
	elem->prev = shell->history;
	if (elem->prev != NULL)
		elem->prev->next = elem;
	elem->next = NULL;
	shell->history = elem;
	shell->history_off = NULL;
}

const char	*peek_history_prev(t_shell *shell)
{
	t_history	*elem;

	elem = shell->history_off;
	if (elem == NULL)
		elem = shell->history;
	else
		elem = elem->prev;
	if (elem == NULL)
		return (NULL);
	shell->history_off = elem;
	return (elem->content);
}

const char	*peek_history_next(t_shell *shell)
{
	t_history	*elem;

	elem = shell->history_off;
	if (elem == NULL)
		return (NULL);
	shell->history_off = elem->next;
	if (elem->next == NULL)
		return (NULL);
	return (elem->next->content);
}
