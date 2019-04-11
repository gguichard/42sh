/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tcollard <tcollard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/04/05 21:08:24 by tcollard          #+#    #+#             */
/*   Updated: 2019/04/11 13:02:39 by fwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "options.h"
#include "shell.h"
#include "vars.h"
#include "builtins.h"
#include "error.h"
#include "hashtable.h"

static void	display_env_vars(t_list *vars)
{
	t_var	*var;
	char	*escaped_str;

	while (vars)
	{
		var = (t_var *)vars->content;
		if (var->is_env)
		{
			if (var->value != NULL)
			{
				escaped_str = ft_strreplace(var->value, "\'", "\'\\\'\'");
				ft_printf("export %s=\'%s\'\n", var->key, escaped_str);
				free(escaped_str);
			}
			else
				ft_printf("export %s\n", var->key);
		}
		vars = vars->next;
	}
}

static int	export_builtin_var(t_alloc *alloc, const char *key
		, const char *value)
{
	t_var	*var;

	if (!is_var_valid_identifier(key))
		return (0);
	if ((var = get_var(alloc->vars, key)) != NULL)
	{
		if (var->tmp_value != NULL)
			ft_strdel(&var->tmp_value);
		if (value == NULL)
			value = var->value;
		var->is_env = 1;
	}
	if (var == NULL || value != NULL)
		update_var(&alloc->vars, key, value);
	if (value != NULL && ft_strequ(key, "PATH"))
		delete_hashentries(alloc->exectable);
	return (1);
}

static int	export_vars(t_alloc *alloc, char **argv, int idx)
{
	int		ret;
	char	*tmp;

	ret = 0;
	while (argv[idx] != NULL)
	{
		if ((tmp = ft_strchr(argv[idx], '=')) != NULL)
			*tmp = '\0';
		if (!export_builtin_var(alloc, argv[idx], tmp != NULL ? tmp + 1 : NULL))
		{
			ret = 1;
			ft_dprintf(STDERR_FILENO
					, "42sh: export: '%s': not a valid identifier\n"
					, argv[idx]);
		}
		idx++;
	}
	return (ret);
}

int			builtin_export(t_ast *elem, t_alloc *alloc)
{
	t_opts	opts;

	parse_opts(&opts, elem->input, "p");
	if (opts.error != 0)
	{
		ft_dprintf(STDERR_FILENO, "42sh: export: -%c: invalid option\n"
				"export: usage: [name[=value] ...] or export -p\n"
				, opts.error);
		return (1);
	}
	if (elem->input[opts.index] == NULL)
	{
		display_env_vars(alloc->vars);
		return (0);
	}
	return (export_vars(alloc, elem->input, opts.index));
}
