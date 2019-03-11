/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cursor_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 16:00:18 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/11 16:42:41 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "libft.h"
#include "cmdline.h"

int	set_cursor_pos(t_cursor *cursor)
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
		cursor->y = ft_strtol(&buffer[2], &endptr, 10);
		if (*endptr == ';')
			cursor->x = ft_strtol(&endptr[1], &endptr, 10);
		return (1);
	}
	return (0);
}
