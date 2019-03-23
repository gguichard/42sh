/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cmdline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 19:50:30 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/22 16:39:28 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <term.h>
#include "cmdline.h"

void		print_next_line_tcaps(void)
{
	static char	*cr_tcap = NULL;
	static char	*do_tcap = NULL;
	static char	*ce_tcap = NULL;

	if (cr_tcap == NULL)
		cr_tcap = tgetstr("cr", NULL);
	if (do_tcap == NULL)
		do_tcap = tgetstr("do", NULL);
	if (ce_tcap == NULL)
		ce_tcap = tgetstr("ce", NULL);
	tputs(cr_tcap, 1, t_putchar);
	tputs(do_tcap, 1, t_putchar);
	tputs(ce_tcap, 1, t_putchar);
}

static void	write_with_colors(const char *str)
{
	static int	color = 1;
	char		*color_str;

	while (*str != '\0')
	{
		ft_asprintf(&color_str, "\033[38;5;%dm", color);
		if (color_str != NULL)
		{
			write(STDOUT_FILENO, color_str, ft_strlen(color_str));
			free(color_str);
		}
		write(STDOUT_FILENO, str, 1);
		write(STDOUT_FILENO, "\033[0m", 4);
		color = (color % 232) + 1;
		str++;
	}
}

void		print_cmdline_str(t_cmdline *cmdline, const char *buffer
		, size_t len)
{
	if (cmdline->konami_code)
		write_with_colors(buffer);
	else
		write(STDOUT_FILENO, buffer, len);
}

int			print_big_cmdline_prompt(t_cmdline *cmdline)
{
	int			is_at_position;
	int			offset;

	is_at_position = 0;
	if (cmdline->row > cmdline->cursor.y)
	{
		go_to_cursor_pos((t_cursor){0, 0});
		offset = 0;
		if (cmdline->visual.toggle)
			offset += ft_max(write(STDOUT_FILENO, "(visual) ", 9), 0);
		offset += ft_max(write(STDOUT_FILENO, ">... ", 5), 0);
		cmdline->prompt.big_offset = offset;
		while (cmdline->cursor.y < 0)
		{
			if (!handle_cursor_down(cmdline))
				break ;
			is_at_position = 1;
		}
		if (!is_at_position)
		{
			is_at_position = 1;
			go_to_cursor_pos(cmdline->cursor);
		}
	}
	return (is_at_position);
}
