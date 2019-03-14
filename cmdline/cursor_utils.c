/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 16:00:18 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/14 12:33:04 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include <term.h>
#include "cmdline.h"

int		set_cursor_pos(t_cursor *cursor)
{
	char		buffer[32];
	ssize_t		ret;
	char		*endptr;

	if (write(STDIN_FILENO, "\033[6n", 4) == -1)
		return (0);
	ret = read(STDIN_FILENO, buffer, sizeof(buffer));
	if (ret == -1 || ret > sizeof(buffer))
		return (0);
	if (ft_strnequ(buffer, "\033[", 2) && buffer[ret - 1] == 'R')
	{
		cursor->y = ft_strtol(&buffer[2], &endptr, 10) - 1;
		if (*endptr == ';')
			cursor->x = ft_strtol(&endptr[1], &endptr, 10) - 1;
		return (1);
	}
	return (0);
}

void	go_to_cursor_pos(t_cursor cursor)
{
	static char	*cm_str = NULL;

	if (cm_str == NULL)
		cm_str = tgetstr("cm", NULL);
	tputs(tgoto(cm_str, cursor.x, cursor.y), 1, t_putchar);
}
