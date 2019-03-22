/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_hooks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/21 10:31:27 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/22 16:15:39 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <term.h>
#include "cmdline.h"
#include "history.h"

static void	fill_input_buffer(t_cmdline *cmdline, const char *entry)
{
	size_t	idx;
	size_t	offset;

	idx = 0;
	offset = 0;
	if (entry != NULL)
	{
		while (entry[offset] != '\0')
		{
			//todo: utf8 historique
			cmdline->input.buffer[idx] = (wint_t)entry[offset];
			idx++;
			offset++;
		}
	}
	cmdline->input.buffer[idx] = L'\0';
	cmdline->input.size = idx;
	cmdline->input.offset = idx;
}

static void	load_entry_in_cmdline(t_cmdline *cmdline, const char *entry)
{
	fill_input_buffer(cmdline, entry);
	cmdline->cursor = (t_cursor){cmdline->prompt.offset
		, ft_max(0, cmdline->cursor.y - cmdline->row)};
	go_to_cursor_pos(cmdline->cursor);
	tputs(tgetstr("cd", NULL), 1, t_putchar);
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
