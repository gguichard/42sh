#include "shell.h"
#include "vars.h"
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

static int	update_pwd(char *dir, t_list **vars, int options, char *buf)
{
	const char	*value;
	t_var		*tmp;

	value = buf;
	tmp = get_var(*vars, "PWD");
	if (tmp != NULL)
		value = tmp->value;
	update_var(vars, "OLDPWD", value);
	update_var(vars, "PWD", (options == 2) ? buf : dir);
	ft_memdel((void **)&dir);
	ft_memdel((void **)&buf);
	g_ret[0] = 0;
	return (0);
}

int			cd_builtins(t_ast *elem, t_alloc *alloc)
{
	int			i;
	int			options;
	char		*buf_pwd;
	char		*dir;

	options = 0;
	if ((i = check_options(elem, &options, alloc)) == -1)
		return (1);
	buf_pwd = getcwd(0, PATH_MAX);
	if (get_var(alloc->vars, "PWD") == NULL)
		create_var(&alloc->vars, "PWD", buf_pwd, 1);
	dir = cd_predef(elem->input[i], alloc->vars, options, buf_pwd);
	if (!dir)
		dir = get_dir(get_var_value(alloc->vars, "PWD")
				, ft_strsplit(elem->input[i], '/'), options, buf_pwd);
	ft_memdel((void **)&buf_pwd);
	if ((ft_strcmp(dir, "") != 0 && check_access(dir, elem->input[i]) == -1)
			|| !ft_strcmp(dir, ""))
	{
		(!ft_strcmp(dir, "")) ? ft_memdel((void **)&dir) : 0;
		return (1);
	}
	(ft_strcmp(elem->input[i], "-") == 0) ? ft_printf("%s\n", dir) : 0;
	return (update_pwd(dir, &alloc->vars, options, getcwd(NULL, PATH_MAX)));
}
