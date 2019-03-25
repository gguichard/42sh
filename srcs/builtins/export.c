#include <unistd.h>
#include "libft.h"
#include "options.h"
#include "shell.h"
#include "vars.h"
#include "builtins.h"
#include "error.h"

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

static int	export_vars(t_list **vars, char **argv, int idx)
{
	int		ret;
	char	*tmp;
	t_var	*var;

	ret = 0;
	while (argv[idx] != NULL)
	{
		if ((tmp = ft_strchr(argv[idx], '=')) != NULL)
			*tmp = '\0';
		if (is_var_valid_identifier(argv[idx]))
		{
			if ((var = get_var(*vars, argv[idx])) != NULL)
				var->is_env = 1;
			update_var(vars, argv[idx], (tmp == NULL) ? NULL : tmp + 1);
		}
		else
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

int			export_builtins(t_ast *elem, t_alloc *alloc)
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
	return (export_vars(&alloc->vars, elem->input, opts.index));
}
