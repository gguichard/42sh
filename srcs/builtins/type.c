#include <unistd.h>
#include "printf.h"
#include "shell.h"
#include "builtins.h"
#include "exectable.h"
#include "aliastable.h"
#include "search_exec.h"

static int	search_type(t_alloc *alloc, const char *elem)
{
	int			ret;
	const char	*path;
	char		*freed_path;
	t_error		error;

	if ((path = get_alias(alloc->aliastable, elem)) != NULL)
		ret = ft_printf("%s is aliased to `%s`\n", elem, path);
	else if ((path = get_exec_path(alloc->exectable, elem, 0)) != NULL)
		ret = ft_printf("%s is hashed (%s)\n", elem, path);
	else if (is_builtins(alloc, elem) == 1)
		ret = ft_printf("%s is a shell builtin\n", elem);
	else if (ft_strchr(elem, '/') != NULL && access(elem, F_OK | X_OK) == 0)
		ret = ft_printf("%s is %s\n", elem, elem);
	else if (ft_strchr(elem, '/') == NULL
			&& (freed_path = search_exec(*(alloc->var), elem, &error)) != NULL)
	{
		ret = ft_printf("%s is %s\n", elem, freed_path);
		free(freed_path);
	}
	else
	{
		ft_dprintf(STDERR_FILENO, "42sh: type: %s: not found\n", elem);
		ret = 1;
	}
	return (ret < 0 ? 0 : 1);
}

int			builtin_type(t_ast *elem, t_alloc *alloc)
{
	int		idx;
	int		ret;

	idx = 1;
	ret = 0;
	while (elem->input[idx] != NULL)
	{
		if (!search_type(alloc, elem->input[idx]))
			ret = 1;
		idx++;
	}
	return (ret);
}
