/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jocohen <jocohen@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/24 10:31:12 by jocohen           #+#    #+#             */
/*   Updated: 2019/03/07 14:32:02 by tcollard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		main(int ac, char **av, char **env)
{
	t_env		*lst;
	t_alloc		al;
	int			i;

	(void)ac;
	(void)av;
	i = 0;
	g_ret[0] = 0;
	g_ret[1] = 0;
	g_in_exec = 0;
	g_clip = 0;
	g_pid = 0;
	g_resize = 0;
	lst = 0;
	al.ast = 0;
	al.env = 0;
	al.history = 0;
	al.input = 0;
	while (i < 10)
		al.fd[i++] = -1;
	env_cp(env, &lst);
	al.env = &lst;
	prompt(&al);
	del_lst_env(al.env);
	return (0);
}
