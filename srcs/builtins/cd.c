/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gguichar <gguichar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/25 20:20:40 by gguichar          #+#    #+#             */
/*   Updated: 2019/04/04 12:11:37 by gguichar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

static char			*get_path_or_cwd(t_list *vars, const char *name)
{
	t_var	*var;

	var = get_var(vars, name);
	if (var == NULL)
		return (getcwd(NULL, PATH_MAX));
	else
		return (ft_strdup(var->value));
}

static int			change_dir(t_alloc *alloc, t_opts *opts
		, const char *cur_path, const char *operand)
{
	char		*pwd;
	t_error		error;
	char		*freed_path;
	const char	*new_pwd_path;

	if (chdir(cur_path) == -1)
	{
		error = check_dir_rights(cur_path, X_OK);
		ft_dprintf(STDERR_FILENO, "cd: %s: %s\n", operand, error_to_str(error));
		return (1);
	}
	pwd = get_path_or_cwd(alloc->vars, "PWD");
	if (pwd != NULL)
		update_var(&alloc->vars, "OLDPWD", pwd);
	free(pwd);
	freed_path = has_opt(opts, 'P') ? getcwd(0, PATH_MAX) : NULL;
	if ((new_pwd_path = has_opt(opts, 'P') ? freed_path : cur_path) != NULL)
		update_var(&alloc->vars, "PWD", new_pwd_path);
	free(freed_path);
	return (0);
}

static char			*get_cur_path(t_alloc *alloc, t_opts *opts
		, const char *operand)
{
	char	*cur_path;
	char	*pwd;

	if (operand[0] == '/')
		cur_path = ft_strdup(operand);
	else
	{
		cur_path = NULL;
		pwd = get_path_or_cwd(alloc->vars, "PWD");
		if (!ft_strequ(operand, ".")
				&& !ft_strequ(operand, "..")
				&& !ft_strnequ(operand, "./", 2)
				&& !ft_strnequ(operand, "../", 3))
			cur_path = search_in_cd_path(alloc, operand, pwd);
		if (cur_path != NULL)
			ft_printf("%s\n", cur_path);
		else
			cur_path = join_path(pwd, operand);
		free(pwd);
	}
	if (cur_path != NULL && !has_opt(opts, 'P'))
		set_dir_to_canonical_form(cur_path);
	return (cur_path);
}

static const char	*get_operand_with_replacement(t_alloc *alloc
		, const char *operand)
{
	if (operand == NULL)
	{
		operand = get_var_value_or_null(alloc->vars, "HOME");
		if (operand == NULL)
			ft_dprintf(STDERR_FILENO, "42sh: cd: HOME not set\n");
	}
	else if (ft_strequ(operand, "-"))
	{
		operand = get_var_value_or_null(alloc->vars, "OLDPWD");
		if (operand == NULL)
			ft_dprintf(STDERR_FILENO, "42sh: cd: OLDPWD not set\n");
	}
	return (operand);
}

int					builtin_cd(t_ast *elem, t_alloc *alloc)
{
	t_opts		opts;
	const char	*operand;
	char		*cur_path;
	int			ret;

	if (parse_opts(&opts, elem->input, "{LP}")->error != 0)
	{
		ft_dprintf(STDERR_FILENO, "42sh: cd: -%c: invalid option\n"
				"cd: usage: cd [-L|-P] [dir]\n", opts.error);
		return (2);
	}
	operand = get_operand_with_replacement(alloc, elem->input[opts.index]);
	if (operand == NULL)
		return (1);
	cur_path = get_cur_path(alloc, &opts, operand);
	if (cur_path == NULL && (ret = 1))
		ft_dprintf(STDERR_FILENO, "42sh: cd: unexpected error\n");
	else
	{
		ret = change_dir(alloc, &opts, cur_path, operand);
		if (ret == 0 && ft_strequ(elem->input[opts.index], "-"))
			ft_printf("%s\n", cur_path);
		free(cur_path);
	}
	return (ret);
}
