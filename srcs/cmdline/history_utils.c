/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/23 20:08:47 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/23 21:52:27 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pwd.h>
#include <unistd.h>
#include "libft.h"
#include "shell.h"
#include "builtins.h"

char	*get_history_file_path(t_alloc *alloc)
{
	char			*home_dir;
	const char		*login;
	struct passwd	*user;
	char			*tab[3];

	home_dir = get_env_value(*alloc->var, "$HOME");
	if (home_dir == NULL || home_dir[0] == '\0')
	{
		login = getlogin();
		if (login != NULL)
		{
			user = getpwnam(login);
			home_dir = user->pw_dir;
		}
	}
	if (home_dir == NULL || home_dir[0] == '\0')
		return (NULL);
	tab[0] = home_dir;
	tab[1] = ".21sh_history";
	tab[2] = NULL;
	return (ft_join(tab, home_dir[ft_strlen(home_dir) - 1] == '/' ? "" : "/"));
}
