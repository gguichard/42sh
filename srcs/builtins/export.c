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

	while (vars)
	{
		var = (t_var *)vars->content;
		if (var->is_env)
		{
			if (var->value != NULL)
				ft_printf("export %s=\"%s\"\n", var->key, var->value);
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
		var->is_env = 1;
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
	if (has_opt(&opts, 'p') && elem->input[opts.index] == NULL)
	{
		display_env_vars(alloc->vars);
		return (0);
	}
	return (export_vars(alloc, elem->input, opts.index));
}
