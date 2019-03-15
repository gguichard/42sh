#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "builtins.h"
#include "parser_lexer.h"
#include "convert_path_to_tab.h"

/*
** Retourne le nombre de path dans la variable path.
*/

static size_t	count_subpath(const char *path)
{
	size_t	subpath_count;

	subpath_count = 1;
	while (*path != '\0')
	{
		if (*path == ':')
			++subpath_count;
		++path;
	}
	return (subpath_count);
}

static int		process_cur_path(const char **path_cpy, char **cur_path
		, t_var *var_lst)
{
	char	*next_sep;

	next_sep = ft_strchr(*path_cpy, ':');
	if (next_sep == *path_cpy || **path_cpy == '\0')
		*cur_path = ft_strdup(".");
	else
	{
		if (next_sep != NULL)
			*next_sep = '\0';
		if ((*cur_path = ft_strdup(*path_cpy)) != NULL)
			expand_home_shortcut(cur_path, var_lst);
	}
	if (*cur_path == NULL)
		return (0);
	if (next_sep != NULL)
		*path_cpy = next_sep + 1;
	return (1);
}

char			**convert_path_to_tab(t_var *var_lst)
{
	const char	*path_base;
	size_t		path_count;
	char		**path_tab;
	size_t		path_idx;

	path_base = get_env_value(var_lst, "PATH");
	path_count = count_subpath(path_base);
	if ((path_tab = (char**)malloc(sizeof(char*) * (path_count + 1))) == NULL)
		return (NULL);
	path_tab[path_count] = NULL;
	path_idx = 0;
	while (path_idx < path_count)
	{
		if (!process_cur_path(&path_base, path_tab + path_idx, var_lst))
			return (ft_strtab_free(path_tab));
		++path_idx;
	}
	return (path_tab);
}
