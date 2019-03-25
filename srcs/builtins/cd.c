#include <unistd.h>
#include "libft.h"
#include "options.h"
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
	ft_strdel(&dir);
	return (-1);
}

static int	update_pwd(char *dir, t_list **vars, t_opts *opts, char *buf)
{
	const char	*value;
	t_var		*tmp;

	value = buf;
	tmp = get_var(*vars, "PWD");
	if (tmp != NULL)
		value = tmp->value;
	update_var(vars, "OLDPWD", value);
	update_var(vars, "PWD", has_opt(opts, 'P') ? buf : dir);
	ft_strdel(&dir);
	ft_strdel(&buf);
	g_ret[0] = 0;
	return (0);
}

int			cd_builtins(t_ast *elem, t_alloc *alloc)
{
	t_opts	opts;
	char	*buf_pwd;
	char	*dir;

	parse_opts(&opts, elem->input, "{LP}");
	if (opts.error != 0)
	{
		ft_dprintf(STDERR_FILENO, "42sh: cd: -%c: invalid option\n"
				"cd: usage: cd [-L|-P] [dir]", opts.error);
		return (2);
	}
	buf_pwd = getcwd(NULL, PATH_MAX);
	if (get_var(alloc->vars, "PWD") == NULL)
		create_var(&alloc->vars, "PWD", buf_pwd, 1);
	dir = cd_predef(elem->input[opts.index], alloc->vars, &opts, buf_pwd);
	if (dir == NULL)
	{
		dir = get_dir(get_var_value(alloc->vars, "PWD")
				, ft_strsplit(elem->input[opts.index], '/'), &opts, buf_pwd);
	}
	ft_strdel(&buf_pwd);
	if ((ft_strcmp(dir, "") != 0
				&& check_access(dir, elem->input[opts.index]) == -1)
			|| !ft_strcmp(dir, ""))
	{
		(!ft_strcmp(dir, "")) ? ft_strdel(&dir) : 0;
		return (1);
	}
	if (ft_strequ(elem->input[opts.index], "-"))
		ft_printf("%s\n", dir);
	return (update_pwd(dir, &alloc->vars, &opts, getcwd(NULL, PATH_MAX)));
}
