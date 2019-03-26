/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 16:00:18 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/26 10:08:43 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>
#include <term.h>
#include <termios.h>
#include "cmdline.h"

int			set_cursor_pos(t_cursor *cursor)
{
	ssize_t	ret;
	char	buffer[32];
	char	*endptr;

	tcflush(STDIN_FILENO, TCIFLUSH);
	if (write(STDIN_FILENO, "\033[6n", 4) == -1)
		return (0);
	ft_memset(buffer, 0, sizeof(buffer));
	if ((ret = read(STDIN_FILENO, buffer, sizeof(buffer))) <= 0)
		return (0);
	if (buffer[0] != '\033' || buffer[1] != '[' || buffer[ret - 1] != 'R')
		return (0);
	endptr = buffer;
	cursor->y = ft_strtol(endptr + 2, &endptr, 10) - 1;
	cursor->x = ft_strtol(endptr + 1, &endptr, 10) - 1;
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
		tputs(tgetstr("cd", NULL), winsize.ws_row - cursor.y, t_putchar);
		go_to_cursor_pos(cursor);
	}
}
