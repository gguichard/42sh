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

void	clean_slash_path(char *s)
{
	int		x;

	x = 0;
	while (s && s[x])
	{
		if (s[x] == '/' && s[x + 1] == '/')
			ft_memmove(s + x, s + x + 1, ft_strlen(s + x));
		else
			x += 1;
	}
}

char	*get_dir(char *pwd, char **tab_path, int options, char *buf_pwd)
{
	char	*dir;
	int		i;

	i = 0;
	dir = ft_strdup(pwd);
	while (tab_path && tab_path[i])
	{
		if (ft_strcmp(tab_path[i], "..") == 0
				&& ft_strcmp(tab_path[i], ".") == 0 && options == 2)
			ft_memdel((void **)&dir);
		else if (ft_strcmp(tab_path[i], "..") == 0)
		{
			(options == 2) ? dir = ft_strdup(buf_pwd) : 0;
			delete_last_folder(dir);
		}
		else if (ft_strcmp(tab_path[i], ".") == 0)
			(options == 2) ? dir = ft_strdup(buf_pwd) : 0;
		else
			add_new_folder(&dir, tab_path[i]);
		if (!dir)
			ft_exit_malloc();
		i += 1;
	}
	ft_strtab_free(tab_path);
	return (dir);
}

char	*cd_predef(char *elem, t_list *vars, int options, char *buf)
{
	char	*dir;

	dir = 0;
	clean_slash_path(elem);
	if (elem && ft_strcmp(elem, "-") == 0)
		(ft_strcmp((dir = ft_strdup(get_var_value(vars, "OLDPWD"))), "") != 0)
			? 0 : error_cd("OLDPWD", 2);
	else if (!elem)
		(ft_strcmp((dir = ft_strdup(get_var_value(vars, "HOME"))), "") != 0)
			? 0 : error_cd("HOME", 2);
	else if (elem[0] == '/')
		dir = cd_slash(elem, options, buf);
	else
		return (0);
	if (!dir)
		ft_exit_malloc();
	return (dir);
}
