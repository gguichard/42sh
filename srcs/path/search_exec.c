#include <unistd.h>
#include <stdlib.h>
#include "shell.h"
#include "convert_path_to_tab.h"
#include "check_path.h"
#include "search_exec.h"

static char		*get_binary_path(const char *path, const char *binary)
{
	size_t	path_len;
	size_t	binary_len;
	char	*file_path;

	path_len = ft_strlen(path);
	binary_len = ft_strlen(binary);
	file_path = (char *)malloc(path_len + binary_len + 2);
	if (file_path == NULL)
		return (NULL);
	ft_memcpy(file_path, path, path_len);
	file_path[path_len] = '/';
	ft_memcpy(&(file_path[path_len + 1]), binary, binary_len + 1);
	return (file_path);
}

static t_error	check_is_valid_path(const char *file_path)
{
	t_error	error;

	error = check_file_rights(file_path, FT_FILE, X_OK);
	if (error == ERRC_FILENOTFOUND || error == ERRC_NONEEDEDRIGHTS)
		error = ERRC_CMDNOTFOUND;
	return (error);
}

char			*search_exec(t_var *var_lst, const char *exec_name
		, t_error *error);
{
	char	*file_path;
	char	**path_tab;
	size_t	index;

	file_path = NULL;
	*error = ERRC_CMDNOTFOUND;
	if ((path_tab = convert_path_to_tab(var_lst)) != NULL)
	{
		index = -1;
		while (path_tab[++index] != NULL)
		{
			if ((file_path = get_binary_path(path_tab[index], binary)) == NULL)
			{
				*error = ERRC_UNEXPECTED;
				break ;
			}
			*error = check_is_valid_path(file_path);
			if (*error == ERRC_NOERROR)
				break ;
			ft_strdel(&file_path);
		}
	}
	ft_strtab_free(path_tab);
	return (file_path);
}
