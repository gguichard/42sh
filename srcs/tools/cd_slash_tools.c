#include "shell.h"
#include "builtins.h"
#include "error.h"

char		*cd_slash(char *dir, int options, char *buf_pwd)
{
	char	*path;
	char	**tmp;
	size_t	x;

	x = -1;
	tmp = ft_strsplit(dir + 1, '/');
	(!(path = ft_strdup("/")) || !tmp) ? ft_exit_malloc() : 0;
	while (tmp[++x])
	{
		if (ft_strcmp(tmp[x], "..") == 0
			&& ft_strcmp(tmp[x], ".") == 0 && options == 2)
			ft_memdel((void **)&path);
		else if (ft_strcmp(tmp[x], "..") == 0)
		{
			(options == 2) ? path = ft_strdup(buf_pwd) : 0;
			delete_last_folder(path);
		}
		else if (ft_strcmp(tmp[x], ".") == 0)
			(options == 2) ? path = ft_strdup(buf_pwd) : 0;
		else
			add_new_folder(&path, tmp[x]);
		(!dir) ? ft_exit_malloc() : 0;
	}
	ft_strtab_free(tmp);
	return (path);
}
