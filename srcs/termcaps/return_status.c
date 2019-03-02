/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   return_status.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/02/08 18:44:12 by jocohen           #+#    #+#             */
/*   Updated: 2019/02/22 20:21:52 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		ret_status(void)
{
	int		err;

	err = 0;
	if (!g_ret[1])
		return ((unsigned char)g_ret[0]);
	if (WIFEXITED(g_ret[0]))
		err = WEXITSTATUS(g_ret[0]);
	else if (WIFSIGNALED(g_ret[0]))
		err = WTERMSIG(g_ret[0]) + 128;
	else if (WIFSTOPPED(g_ret[0]))
		err = WSTOPSIG(g_ret[0]) + 128;
	g_ret[0] = err;
	g_ret[1] = 0;
	return (err);
}
