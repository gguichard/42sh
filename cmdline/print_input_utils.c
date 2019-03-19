/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_input_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/18 19:50:30 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/19 16:20:42 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <stdlib.h>
#include <wchar.h>
#include <term.h>
#include "cmdline.h"

void	print_go_next_line(void)
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

void	print_mbstr(const wchar_t *buffer, size_t len)
{
	char	*str;
	int		offset;
	int		wlen;

	str = (char *)malloc(len * sizeof(wint_t));
	if (str == NULL)
		write(STDOUT_FILENO, buffer, len * sizeof(wint_t));
	else
	{
		offset = 0;
		while (len > 0)
		{
			wlen = ft_wcharlen(*buffer);
			ft_memcpy(str + offset, buffer, wlen);
			offset += wlen;
			buffer++;
			len--;
		}
		write(STDOUT_FILENO, str, offset);
		free(str);
	}
}

void	print_big_line_indicator(t_cmdline *cmdline)
{
	go_to_cursor_pos((t_cursor){0, 0});
	write(STDOUT_FILENO, ">... ", 5);
	while (cmdline->cursor.y < 0)
	{
		if (!handle_cursor_down(cmdline))
			break ;
	}
	go_to_cursor_pos(cmdline->cursor);
}
