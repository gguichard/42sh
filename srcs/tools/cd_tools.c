#include "libft.h"
#include "options.h"
#include "shell.h"
#include "vars.h"
#include "builtins.h"
#include "error.h"

char	*delete_last_folder(char *dir)
{
	int	i;

	i = ft_strlen(dir) - 1;
	if (i < 0)
		return (dir);
	while (i > 0 && dir[i] != '/')
	{
		dir[i] = '\0';
		i -= 1;
	}
	if (i != 0)
		dir[i] = '\0';
	return (dir);
}

void	add_new_folder(char **dir, char *folder)
{
	char	*new_dir;

	new_dir = ft_strjoin(*dir, ((*dir)[ft_strlen(*dir) - 1] == '/') ? 0 : "/");
	ft_memdel((void **)dir);
	*dir = ft_strjoin(new_dir, folder);
	if (!*dir || !new_dir)
		ft_exit_malloc();
	ft_memdel((void **)&new_dir);
}

static void	clean_slash_path(char *str)
{
	int	idx;

	idx = 0;
	while (str[idx] != '\0')
	{
		if (str[idx] == '/' && str[idx + 1] == '/')
			ft_memcpy(str + idx, str + idx + 1, ft_strlen(str + idx));
		else
			idx += 1;
	}
}

char	*get_dir(char *pwd, char **tab_path, t_opts *opts, char *buf_pwd)
{
	char	*dir;
	int		i;

	i = 0;
	dir = ft_strdup(pwd);
	while (tab_path && tab_path[i])
	{
		if (ft_strcmp(tab_path[i], "..") == 0
				&& ft_strcmp(tab_path[i], ".") == 0 && has_opt(opts, 'P'))
			ft_memdel((void **)&dir);
		else if (ft_strcmp(tab_path[i], "..") == 0)
		{
			has_opt(opts, 'P') ? dir = ft_strdup(buf_pwd) : 0;
			delete_last_folder(dir);
		}
		else if (ft_strcmp(tab_path[i], ".") == 0)
			has_opt(opts, 'P') ? dir = ft_strdup(buf_pwd) : 0;
		else
			add_new_folder(&dir, tab_path[i]);
		if (!dir)
			ft_exit_malloc();
		i += 1;
	}
	ft_strtab_free(tab_path);
	return (dir);
}

char	*cd_predef(char *elem, t_list *vars, t_opts *opts, char *buf)
{
	char	*dir;

	if (elem != NULL)
		clean_slash_path(elem);
	dir = NULL;
	if (elem != NULL && ft_strequ(elem, "-"))
	{
		dir = ft_strdup(get_var_value(vars, "OLDPWD"));
		if (dir == NULL || dir[0] == '\0')
			error_cd("OLDPWD", 2);
	}
	else if (elem == NULL)
	{
		dir = ft_strdup(get_var_value(vars, "HOME"));
		if (dir == NULL || dir[0] == '\0')
			error_cd("HOME", 2);
	}
	else if (elem[0] == '/')
		dir = cd_slash(elem, opts, buf);
	else
		return (NULL);
	return (dir);
}
