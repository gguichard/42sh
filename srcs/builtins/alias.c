#include "shell.h"
#include "options.h"
#include "aliastable.h"
#include "builtins.h"

int		alias_builtins(t_ast *elem, t_alloc *alloc)
{
	t_opts	opts;

	(void)alloc;
	parse_opts(&opts, elem->input, "");
	if (opts.error != 0)
	{
		ft_dprintf(STDERR_FILENO, "42sh: alias: -%c: invalid option\n"
				, opts.error);
		ft_dprintf(STDERR_FILENO
				, "42sh: alias: usage: alias [name[=value] ...]\n");
		return (1);
	}
	/*if (elem->input[opts->index] == NULL)
		return (!print_aliastable(alloc->aliastable));
	return (!add_or_print_aliases(elem->input + opts->index
				, alloc->var, alloc->exectable));*/
	return (0);
}
