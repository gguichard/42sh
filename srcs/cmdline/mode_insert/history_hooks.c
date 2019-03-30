/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_hooks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 10:31:27 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/30 20:21:13 by gguichar         ###   ########.fr       */
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
	static char	*ce_tcap = NULL;
	static char	*cr_tcap = NULL;
	static char	*do_tcap = NULL;
	static char	*cd_tcap = NULL;
	t_cursor	home_cursor;

	if (ce_tcap == NULL)
		ce_tcap = tgetstr("ce", NULL);
	if (cr_tcap == NULL)
		cr_tcap = tgetstr("cr", NULL);
	if (do_tcap == NULL)
		do_tcap = tgetstr("do", NULL);
	if (cd_tcap == NULL)
		cd_tcap = tgetstr("cd", NULL);
	fill_input_buffer(cmdline, entry);
	home_cursor = (t_cursor){cmdline->prompt.offset, 0};
	go_to_cursor_pos(cmdline, home_cursor);
	tputs(ce_tcap, 1, t_putchar);
	tputs(cr_tcap, 1, t_putchar);
	tputs(do_tcap, 1, t_putchar);
	tputs(cd_tcap, 1, t_putchar);
	cmdline->cursor.x = 0;
	cmdline->cursor.y += 1;
	go_to_cursor_pos(cmdline, home_cursor);
	print_cmdline(cmdline);
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
