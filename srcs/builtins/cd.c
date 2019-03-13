#include "shell.h"
#include "builtins.h"
#include "error.h"

static int	check_access(char *dir, char *folder)
{
	int		err;

	err = -1;
	if (access(dir, F_OK) == -1)
		err = 0;
	else if (access(dir, X_OK) == -1)
		err = 1;
	else if (chdir(dir) == -1)
		err = 3;
	else
		return (0);
	if (folder && folder[0] != '-')
		error_cd(folder, err);
	else
		error_cd(dir, err);
	ft_memdel((void **)&dir);
	return (-1);
}

static int	check_options(t_ast *elem, int *options, t_alloc *alloc)
{
	int	i;
	int	x;

	(void)alloc;
	i = 1;
	while (elem->input[i] && elem->input[i][0] == '-')
	{
		x = 1;
		while (elem->input[i][x] && (elem->input[i][x] == 'P'
		|| elem->input[i][x] == 'L'))
		{
			if (elem->input[i][x] == 'P')
				*options = 2;
			else if (elem->input[i][x] == 'L')
				*options = (*options == 2) ? 2 : 1;
			x += 1;
		}
		if (ft_strcmp(elem->input[i], "-") == 0)
			return (i);
		else if (elem->input[i][x] != '\0')
			return (error_cd(elem->input[i], 0));
		i += 1;
	}
	return (check_arg_cd(elem, i));
}

static int	modif_oldpwd(t_var *lst_env, char *buf)
{
	t_var	*tmp;

	tmp = NULL;
	if ((tmp = find_elem_env(lst_env, "OLDPWD"))
		&& (find_elem_env(lst_env, "PWD")))
	{
		free(tmp->value);
		if (!(tmp->value = ft_strdup(find_elem_env(lst_env, "PWD")->value)))
			ft_exit_malloc();
	}
	else
	{
		if (!(tmp = find_elem_env(lst_env, "PWD")))
			add_elem_env(lst_env, "OLDPWD", buf);
		else
			add_elem_env(lst_env, "OLDPWD", tmp->value);
	}
	return (0);
}

static int	modif_env(char *dir, t_var *lst_env, int options, char *buf)
{
	t_var	*tmp;

	modif_oldpwd(lst_env, buf);
	tmp = find_elem_env(lst_env, "PWD");
	ft_memdel((void **)&(tmp->value));
	tmp->value = ft_strdup((options == 2) ? buf : dir);
	ft_memdel((void **)&dir);
	ft_memdel((void **)&buf);
	g_ret[0] = 0;
	return (0);
}

int			cd_builtins(t_ast *elem, t_var *lst_env, t_alloc *alloc)
{
	int			i;
	int			options;
	char		*buf_pwd;
	char		*dir;

	options = 0;
	if ((i = check_options(elem, &options, alloc)) == -1)
		return (1);
	buf_pwd = getcwd(0, PATH_MAX);
	if (!find_elem_env(lst_env, "PWD"))
		add_elem_env(lst_env, "PWD", buf_pwd);
	dir = cd_predef(elem->input[i], lst_env, options, buf_pwd);
	if (!dir)
		dir = get_dir(get_env_value(lst_env, "$PWD"),
		ft_strsplit(elem->input[i], '/'), options, buf_pwd);
	ft_memdel((void **)&buf_pwd);
	if ((ft_strcmp(dir, "") != 0 && check_access(dir, elem->input[i]) == -1)
		|| !ft_strcmp(dir, ""))
	{
		(!ft_strcmp(dir, "")) ? ft_memdel((void **)&dir) : 0;
		return (1);
	}
	(ft_strcmp(elem->input[i], "-") == 0) ? ft_printf("%s\n", dir) : 0;
	return (modif_env(dir, lst_env, options, getcwd(0, PATH_MAX)));
}
