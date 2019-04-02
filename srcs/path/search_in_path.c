#include <unistd.h>
#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "convert_path_to_tab.h"
#include "check_path.h"

static t_error	check_is_valid_path(const char *file_path, int rights)
{
	t_error	error;

	error = check_file_rights(file_path, FT_FILE, rights);
	if (error != ERRC_NOERROR && error != ERRC_UNEXPECTED)
		error = ERRC_FILENOTFOUND;
	return (error);
}

char			*search_in_path(t_list *vars, const char *file_name, int rights
		, t_error *error)
{
	char	*file_path;
	char	**path_tab;
	size_t	index;

	file_path = NULL;
	*error = ERRC_FILENOTFOUND;
	if ((path_tab = convert_path_to_tab(vars)) != NULL)
	{
		index = -1;
		while (path_tab[++index] != NULL)
		{
			if ((file_path = join_path(path_tab[index], file_name)) == NULL)
			{
				*error = ERRC_UNEXPECTED;
				break ;
			}
			*error = check_is_valid_path(file_path, rights);
			if (*error == ERRC_NOERROR)
				break ;
			ft_strdel(&file_path);
		}
	}
	ft_strtab_free(path_tab);
	return (file_path);
}
