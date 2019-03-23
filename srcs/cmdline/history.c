/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 10:18:39 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/23 21:49:01 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "libft.h"
#include "shell.h"
#include "history.h"

int			load_history_file_entries(t_alloc *alloc, t_history *history)
{
	char	*file_path;
	int		fd;
	char	*line;

	file_path = get_history_file_path(alloc);
	if (file_path == NULL)
		return (0);
	fd = open(file_path, O_RDONLY);
	if (fd != -1)
	{
		// TODO: replace GNL
		while (get_next_line(fd, &line) == 1)
		{
			add_history_entry(history, line);
			free(line);
		}
		close(fd);
	}
	free(file_path);
	return (1);
}

int			save_history_entries(t_alloc *alloc, t_history *history)
{
	char			*file_path;
	int				fd;
	t_history_entry	*entry;

	file_path = get_history_file_path(alloc);
	if (file_path == NULL)
		return (0);
	fd = open(file_path, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd != -1)
	{
		entry = history->front;
		while (entry != NULL)
		{
			write(fd, entry->content, ft_strlen(entry->content));
			write(fd, "\n", 1);
			entry = entry->next;
		}
		close(fd);
	}
	free(file_path);
	return (fd != -1);
}

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

const char	*peek_history_next(t_history *history)
{
	if (history->offset == NULL)
		return (NULL);
	history->offset = history->offset->next;
	if (history->offset == NULL)
		return (NULL);
	return (history->offset->content);
}
