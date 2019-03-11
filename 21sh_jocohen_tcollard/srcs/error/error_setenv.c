/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_setenv.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 12:19:37 by tcollard          #+#    #+#             */
/*   Updated: 2019/02/13 14:48:16 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int	error_setenv(int i)
{
	if (i == 1)
		write(2, "21sh: setenv: usage: setenv [key] [value] [int overwrite]\n",
		58);
	else if (i == 2)
		write(2, "21sh: setenv: no '=' in the name\n", 33);
	else if (i == 3)
	{
		write(2, "21sh: setenv: too much arguments\n", 33);
		write(2, "usage: setenv [key] [value] [int overwrite]\n", 44);
	}
	g_ret[0] = 2;
	return (-1);
}
