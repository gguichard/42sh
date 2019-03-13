#include "shell.h"
#include "builtins.h"
#include "exectable.h"
#include "hashtable.h"

int	type_builtins(t_ast *elem, t_var *lst_env, t_alloc *alloc)
{
	int			i;
	int			ret;
	const char	*path;

	i = 1;
	ret = 0;
	while (elem->input[i])
	{
		if ((path = get_exec_path(alloc->exectable, elem->input[i], 0)))
			ret = ft_printf("%s is hashed (%s)\n", elem->input[i], path);
		else if (is_builtins(alloc, elem->input[i]) == 1)
			ret = ft_printf("%s is a shell builtin\n", elem->input[i]);
		else
		{
			ft_dprintf(2, "42sh: type: %s: not found\n", elem->input[i]);
			ret = 1;
		}
		i += 1;
	}
	if (ret != 1 && ret != -1)
		return (0);
	return (1);
}
