/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_hooks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 10:31:27 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/24 12:48:24 by gguichar         ###   ########.fr       */
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
	fill_input_buffer(cmdline, entry);
	cmdline->cursor = (t_cursor){cmdline->prompt.offset
		, ft_max(0, cmdline->cursor.y - cmdline->row)};
	go_to_cursor_pos(cmdline->cursor);
	clear_after_cursor(cmdline->cursor, cmdline->winsize);
	print_cmdline(cmdline);
}

int			handle_history_prev(t_cmdline *cmdline)
{
	const char	*entry;

	entry = peek_history_prev(&cmdline->history);
	if (entry == NULL)
		return (0);
	load_entry_in_cmdline(cmdline, entry);
	return (1);
}

int			handle_history_next(t_cmdline *cmdline)
{
	const char	*entry;

	entry = peek_history_next(&cmdline->history);
	load_entry_in_cmdline(cmdline, entry);
	return (1);
}
