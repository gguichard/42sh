/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_hooks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 10:31:27 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/30 20:48:34 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <term.h>
#include "cmdline.h"
#include "history.h"

static void	fill_input_buffer(t_cmdline *cmdline, const char *entry)
{
	int	len;

	if (entry == NULL)
		len = 0;
	else
	{
		len = ft_min(ft_strlen(entry), cmdline->input.capacity);
		ft_memcpy(cmdline->input.buffer, entry, len);
	}
	cmdline->input.buffer[len] = '\0';
	cmdline->input.size = len;
	cmdline->input.offset = len;
}

static void	load_entry_in_cmdline(t_cmdline *cmdline, const char *entry)
{
	static char	*cd_tcap = NULL;

	if (cd_tcap == NULL)
		cd_tcap = tgetstr("cd", NULL);
	fill_input_buffer(cmdline, entry);
	go_to_cursor_pos(cmdline, (t_cursor){0, 0});
	tputs(cd_tcap, 1, t_putchar);
	print_prompt_and_cmdline(cmdline);
}

int			handle_history_prev(t_cmdline *cmdline)
{
	t_history	*history;
	const char	*entry;

	history = &cmdline->history;
	entry = peek_history_prev(history);
	if (entry == NULL)
		return (0);
	else if (history->back == history->offset)
	{
		free(history->buffer);
		history->buffer = ft_strdup(cmdline->input.buffer);
	}
	load_entry_in_cmdline(cmdline, entry);
	return (1);
}

int			handle_history_next(t_cmdline *cmdline)
{
	t_history	*history;
	const char	*entry;

	history = &cmdline->history;
	entry = peek_history_next(history);
	if (entry == NULL && history->buffer == NULL)
		return (0);
	if (entry != NULL)
		load_entry_in_cmdline(cmdline, entry);
	else
	{
		load_entry_in_cmdline(cmdline, history->buffer);
		ft_strdel(&history->buffer);
	}
	return (1);
}
