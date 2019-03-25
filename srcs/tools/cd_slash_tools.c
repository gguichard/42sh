#include "libft.h"
#include "options.h"
#include "shell.h"
#include "builtins.h"
#include "error.h"

char		*cd_slash(char *dir, t_opts *opts, char *buf_pwd)
{
	char	*path;
	char	**tmp;
	size_t	idx;

	idx = 0;
	tmp = ft_strsplit(dir + 1, '/');
	(!(path = ft_strdup("/")) || !tmp) ? ft_exit_malloc() : 0;
	while (tmp[idx] != NULL)
	{
		if (ft_strcmp(tmp[x], "..") == 0
			&& ft_strcmp(tmp[x], ".") == 0 && has_opt(opts, 'P'))
			ft_memdel((void **)&path);
		else if (ft_strcmp(tmp[x], "..") == 0)
		{
			has_opt(opts, 'P') ? path = ft_strdup(buf_pwd) : 0;
			delete_last_folder(path);
		}
		else if (ft_strcmp(tmp[x], ".") == 0)
			has_opt(opts, 'P') ? path = ft_strdup(buf_pwd) : 0;
		else
			add_new_folder(&path, tmp[x]);
		(!dir) ? ft_exit_malloc() : 0;
		idx++;
	}
	ft_strtab_free(tmp);
	return (path);
}
