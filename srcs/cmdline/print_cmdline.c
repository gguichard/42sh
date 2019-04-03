/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cmdline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/17 00:13:25 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/03 17:32:21 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <term.h>
#include "cmdline.h"

void	update_cmdline_at_offset(t_cmdline *cmdline, char caller
		, int is_deletion)
{
	static char	*ce_tcap = NULL;
	t_cursor	saved_cursor;

	if (ce_tcap == NULL)
		ce_tcap = tgetstr("ce", NULL);
	if (cmdline->input.offset != cmdline->input.size)
	{
		saved_cursor = cmdline->cursor;
		print_line_by_line(cmdline, cmdline->input.offset);
		if (is_deletion && (caller == '\n' || cmdline->cursor.x == 0))
		{
			if (caller == '\n')
			{
				write(STDOUT_FILENO, "\n", 1);
				cmdline->cursor.x = 0;
				cmdline->cursor.y += 1;
			}
			tputs(ce_tcap, 1, t_putchar);
		}
		go_to_cursor_pos(cmdline, saved_cursor);
	}
}

size_t	count_only_printable_chars(const char *str)
{
	size_t	total;

	total = 0;
	while (*str != '\0')
	{
		if (ft_isprint(*str))
			total++;
		str++;
	}
	return (total);
}

void	print_prompt_and_cmdline(t_cmdline *cmdline)
{
	size_t	total;

	total = 0;
	if (cmdline->visual.toggle)
		total += ft_max(write(STDOUT_FILENO, "(visual) ", 9), 0);
	if (write(STDOUT_FILENO, cmdline->prompt.str
				, ft_strlen(cmdline->prompt.str)) != -1)
		total += count_only_printable_chars(cmdline->prompt.str);
	cmdline->cursor.x = total % ft_max(cmdline->winsize.ws_col, 1);
	cmdline->cursor.y = 0;
	cmdline->prompt.offset = cmdline->cursor.x;
	print_only_cmdline(cmdline);
}
