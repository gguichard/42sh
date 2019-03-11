/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   change_dir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/23 15:13:49 by gguichar          #+#    #+#             */
/*   Updated: 2019/01/30 12:24:00 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "vars.h"
#include "error.h"
#include "check_path.h"
#include "builtins.h"

static char		*get_path_or_cwd(t_shell *shell, const char *name)
{
	t_var	*var;

	var = get_var(shell->env, name);
	if (var == NULL)
		var = get_var(shell->local, name);
	if (var == NULL)
		return (getcwd(NULL, 0));
	return (ft_strdup(var->value));
}

static char		*get_new_cur_path(t_shell *shell, const char *cur_path)
{
	char	*pwd;
	size_t	pwd_len;
	size_t	cur_len;
	char	*new_path;

	if ((cur_path)[0] == '/')
		new_path = ft_strdup(cur_path);
	else
	{
		pwd = get_path_or_cwd(shell, "PWD");
		pwd_len = (pwd == NULL) ? 0 : ft_strlen(pwd);
		if (pwd_len > 0 && pwd[pwd_len - 1] == '/')
			pwd_len -= 1;
		cur_len = ft_strlen(cur_path);
		new_path = (char *)malloc((pwd_len + cur_len + 2) * sizeof(char));
		if (new_path != NULL)
		{
			if (pwd != NULL)
				ft_memcpy(new_path, pwd, pwd_len);
			new_path[pwd_len] = '/';
			ft_memcpy(new_path + pwd_len + 1, cur_path, cur_len + 1);
		}
		free(pwd);
	}
	return (new_path);
}

static char		*get_cur_path(t_shell *shell, int argc, char **argv)
{
	char	*cur_path;
	char	*tmp;

	cur_path = NULL;
	if (argc <= 1)
		cur_path = get_path_or_cwd(shell, "HOME");
	else if (!ft_strequ(argv[1], "-"))
		cur_path = ft_strdup(argv[1]);
	else
		cur_path = get_path_or_cwd(shell, "OLDPWD");
	if (cur_path != NULL)
	{
		tmp = get_new_cur_path(shell, cur_path);
		free(cur_path);
		cur_path = tmp;
		if (cur_path != NULL)
			set_dir_to_canonical_form(cur_path);
	}
	return (cur_path);
}

static int		change_dir(t_shell *shell, const char *cur_path, char **argv)
{
	char		*pwd;
	t_error		error;
	const char	*def_path;

	if (chdir(cur_path) == 0)
	{
		pwd = get_path_or_cwd(shell, "PWD");
		update_var(&(shell->env), "OLDPWD", pwd == NULL ? "" : pwd);
		update_var(&(shell->env), "PWD", cur_path);
		free(pwd);
		return (0);
	}
	error = check_dir_rights(cur_path, X_OK);
	def_path = argv[1];
	if (def_path == NULL)
		def_path = cur_path;
	ft_dprintf(2, "%s: %s: %s\n", argv[0], def_path, error_to_str(error));
	return (1);
}

int				builtin_cd(t_shell *shell, int argc, char **argv)
{
	int		ret;
	char	*cur_path;

	ret = 0;
	cur_path = get_cur_path(shell, argc, argv);
	if (cur_path == NULL)
	{
		ret = 1;
		ft_dprintf(2, "%s: %s: Unexpected error\n", ERR_PREFIX, argv[0]);
	}
	else
	{
		ret = change_dir(shell, cur_path, argv);
		if (ret == 0 && argc > 1 && ft_strequ(argv[1], "-"))
			ft_printf("%s\n", cur_path);
		free(cur_path);
	}
	return (ret);
}
