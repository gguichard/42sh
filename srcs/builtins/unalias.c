#include <unistd.h>
#include "shell.h"
#include "options.h"
#include "aliastable.h"
#include "builtins.h"

static int	remove_these_aliases(char **params, t_hashtable *aliastable)
{
	int		no_error;

	no_error = 1;
	while (*params != NULL)
	{
		if (!remove_hashentry(aliastable, *params))
		{
			ft_dprintf(STDERR_FILENO, "42sh: unalias: %s: not found\n"
					, *params);
			no_error = 0;
		}
		++params;
	}
	return (no_error);
}

int			unalias_builtins(t_ast *elem, t_alloc *alloc)
{
	t_opts	opts;

	parse_opts(&opts, elem->input, "a");
	if (opts.error != 0
			|| (opts.value == 0 && elem->input[opts.index] == NULL))
	{
		if (opts.error != 0)
			ft_dprintf(STDERR_FILENO, "42sh: unalias: -%c: invalid option\n"
					, opts.error);
		ft_dprintf(STDERR_FILENO
				, "42sh: unalias: usage: unalias [-a] name [name ...]\n");
		return (2);
	}
	if (has_opt(&opts, 'a'))
		return (delete_hashentries(alloc->aliastable));
	return (!remove_these_aliases(elem->input + opts.index
				, alloc->aliastable));
}
