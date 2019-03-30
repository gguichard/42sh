/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_cmdline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 19:50:30 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/30 20:00:20 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include "cmdline.h"

void		print_cmdline_str(t_cmdline *cmdline, const char *buffer
		, size_t len)
{
	static int	color = 1;
	size_t		offset;

	cmdline->cursor.x += len;
	cmdline->cursor.y += cmdline->cursor.x / ft_max(1, cmdline->winsize.ws_col);
	cmdline->cursor.x %= ft_max(1, cmdline->winsize.ws_col);
	if (cmdline->cursor.x == 0)
	{
		cmdline->cursor.x = cmdline->winsize.ws_col - 1;
		cmdline->cursor.y -= 1;
	}
	if (!cmdline->konami_code)
		write(STDOUT_FILENO, buffer, len);
	else
	{
		offset = 0;
		while (offset < len)
		{
			ft_printf("\033[38;5;%dm%c\033[0m", color, buffer[offset]);
			color = (color % 232) + 1;
			offset++;
		}
	}
}

void		print_prompt_and_cmdline(t_cmdline *cmdline)
{
	size_t	total;

	total = 0;
	if (cmdline->visual.toggle)
		total += ft_max(write(STDOUT_FILENO, "(visual) ", 9), 0);
	total += ft_max(write(STDOUT_FILENO, cmdline->prompt.str
				, ft_strlen(cmdline->prompt.str)), 0);
	cmdline->cursor.x = total % ft_max(cmdline->winsize.ws_col, 1);
	cmdline->cursor.y = 0;
	cmdline->prompt.offset = cmdline->cursor.x;
	print_cmdline(cmdline);
}
