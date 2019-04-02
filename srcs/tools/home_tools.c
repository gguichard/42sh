/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   home_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/02 14:27:33 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/02 14:46:09 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <pwd.h>
#include "libft.h"
#include "shell.h"
#include "vars.h"

const char	*get_home_directory(t_list *vars)
{
	const char		*home_dir;
	const char		*login;
	struct passwd	*user;

	home_dir = get_var_value(vars, "HOME");
	if (home_dir == NULL || home_dir[0] == '\0')
	{
		login = getlogin();
		if (login != NULL)
		{
			user = getpwnam(login);
			home_dir = user->pw_dir;
		}
	}
	return (home_dir);
}
