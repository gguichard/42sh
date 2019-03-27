/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/25 20:20:40 by gguichar          #+#    #+#             */
/*   Updated: 2019/03/26 14:05:26 by jocohen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>
#include "libft.h"
#include "options.h"
#include "shell.h"
#include "vars.h"
#include "error.h"
#include "check_path.h"
#include "builtins.h"

static char		*get_path_or_cwd(t_alloc *alloc, const char *name)
{
	t_var			*var;
	const char		*login;

	var = get_var(alloc->vars, name);
	if (var == NULL)
	{
		if (ft_strequ(name, "HOME") && (login = getlogin()) != NULL)
			return (ft_strdup(getpwnam(login)->pw_dir));
		return (getcwd(NULL, PATH_MAX));
	}
	return (ft_strdup(var->value));
}

static char		*get_new_cur_path(t_alloc *alloc, const char *cur_path)
{
	char	*pwd;
	size_t	pwd_len;
	size_t	cur_len;
	char	*new_path;

	if (cur_path[0] == '/')
		new_path = ft_strdup(cur_path);
	else
	{
		pwd = get_path_or_cwd(alloc, "PWD");
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

static char		*get_cur_path(t_alloc *alloc, t_opts *opts, const char *operand)
{
	char	*cur_path;
	char	*tmp;

	cur_path = NULL;
	if (operand == NULL)
		cur_path = get_path_or_cwd(alloc, "HOME");
	else if (ft_strequ(operand, "-"))
		cur_path = get_path_or_cwd(alloc, "OLDPWD");
	else
		cur_path = ft_strdup(operand);
	if (cur_path != NULL)
	{
		tmp = get_new_cur_path(alloc, cur_path);
		free(cur_path);
		cur_path = tmp;
		if (cur_path != NULL && !has_opt(opts, 'P'))
			set_dir_to_canonical_form(cur_path);
	}
	return (cur_path);
}

static int		change_dir(t_alloc *alloc, t_opts *opts, const char *cur_path
		, const char *operand)
{
	char		*pwd;
	t_error		error;
	const char	*def_path;
	char		*freed_path;
	const char	*new_pwd_path;

	if (chdir(cur_path) == -1)
	{
		error = check_dir_rights(cur_path, X_OK);
		def_path = operand;
		if (def_path == NULL || ft_strequ(def_path, "-"))
			def_path = cur_path;
		ft_dprintf(STDERR_FILENO, "cd: %s: %s\n", def_path
				, error_to_str(error));
		return (1);
	}
	pwd = get_path_or_cwd(alloc, "PWD");
	if (pwd != NULL)
		update_var(&alloc->vars, "OLDPWD", pwd);
	free(pwd);
	freed_path = has_opt(opts, 'P') ? getcwd(0, PATH_MAX) : NULL;
	if ((new_pwd_path = has_opt(opts, 'P') ? freed_path : cur_path) != NULL)
		update_var(&alloc->vars, "PWD", new_pwd_path);
	free(freed_path);
	return (0);
}

int				builtin_cd(t_ast *elem, t_alloc *alloc)
{
	t_opts	opts;
	int		ret;
	char	*cur_path;

	parse_opts(&opts, elem->input, "{LP}");
	if (opts.error != 0)
	{
		ft_dprintf(STDERR_FILENO, "42sh: cd: -%c: invalid option\n"
				"cd: usage: cd [-L|-P] [dir]", opts.error);
		return (2);
	}
	cur_path = get_cur_path(alloc, &opts, elem->input[opts.index]);
	if (cur_path == NULL)
	{
		ret = 1;
		ft_dprintf(STDERR_FILENO, "42sh: cd: unexpected error\n");
	}
	else
	{
		ret = change_dir(alloc, &opts, cur_path, elem->input[opts.index]);
		if (ret == 0 && ft_strequ(elem->input[opts.index], "-"))
			ft_printf("%s\n", cur_path);
		free(cur_path);
	}
	return (ret);
}
