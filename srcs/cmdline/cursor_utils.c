/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 16:00:18 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/22 18:08:59 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <term.h>
#include "cmdline.h"

static int	stdin_buffer(t_cmdline *cmdline)
{
	size_t	idx;

	idx = 0;
	while (idx < sizeof(cmdline->input.stdin_buffer) - 1)
	{
		if (read(STDIN_FILENO, cmdline->input.stdin_buffer + idx, 1) != 1)
			break ;
		if (idx > 0
				&& cmdline->input.stdin_buffer[idx - 1] == '\033'
				&& cmdline->input.stdin_buffer[idx] == '[')
		{
			cmdline->input.stdin_buffer[--idx] = '\0';
			return (1);
		}
		idx++;
	}
	cmdline->input.stdin_buffer[0] = '\0';
	return (0);
}

int			set_cursor_pos(t_cmdline *cmdline)
{
	size_t	idx;
	char	buffer[32];
	char	*endptr;

	if (write(STDIN_FILENO, "\033[6n", 4) == -1 || !stdin_buffer(cmdline))
		return (0);
	idx = 0;
	while (idx < sizeof(buffer))
	{
		if (read(STDIN_FILENO, buffer + idx, 1) != 1)
			return (0);
		if (buffer[idx] == 'R')
			break ;
		idx++;
	}
	if (buffer[idx] != 'R')
		return (0);
	cmdline->cursor.y = ft_strtol(buffer, &endptr, 10) - 1;
	if (*endptr == ';')
		cmdline->cursor.x = ft_strtol(endptr + 1, &endptr, 10) - 1;
	return (1);
}

void		go_to_cursor_pos(t_cursor cursor)
{
	static char	*cm_str = NULL;

	if (cm_str == NULL)
		cm_str = tgetstr("cm", NULL);
	tputs(tgoto(cm_str, cursor.x, cursor.y), 1, t_putchar);
}

void		clear_after_cursor(t_cursor cursor, struct winsize winsize)
{
	tputs(tgetstr("ce", NULL), 1, t_putchar);
	if ((cursor.y + 1) < winsize.ws_row)
	{
		tputs(tgetstr("cr", NULL), 1, t_putchar);
		tputs(tgetstr("do", NULL), 1, t_putchar);
		tputs(tgetstr("cd", NULL), 1, t_putchar);
		go_to_cursor_pos(cursor);
	}
}
