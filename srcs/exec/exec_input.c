#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "shell.h"
#include "vars.h"
#include "job.h"
#include "error.h"
#include "hashtable.h"
#include "exectable.h"
#include "check_path.h"
#include "execution.h"
#include "signals.h"
// #include "builtins.h" // TODO: deplacer le prototype de wait_pid

static char			*search_exec(t_list *vars, const char *name, int *hashable
		, t_error *err)
{
	char	*path;

	path = search_in_path(vars, name, X_OK, err);
	if (*err != ERRC_NOERROR)
	{
		ft_strdel(&path);
		return (NULL);
	}
	*hashable = 1;
	return (path);
}

static const char	*check_right_alias(const char *alias, t_error *err)
{
	*err = check_file_rights(alias, FT_FILE, X_OK);
	if (*err != ERRC_NOERROR)
		return (NULL);
	return (alias);
}

char				*exec_path(t_alloc *alloc, const char *name, int *hashable
		, t_error *err)
{
	const char	*alias;
	char		*path_exec;

	*err = ERRC_NOERROR;
	path_exec = NULL;
	alias = NULL;
	if (!ft_strchr(name, '/'))
	{
		if ((alias = get_exec_path(alloc->exectable, name, 1)) != NULL)
			path_exec = ft_strdup(check_right_alias(alias, err));
		else
			path_exec = search_exec(alloc->vars, name, hashable, err);
	}
	else
	{
		if ((*err = check_file_rights(name, FT_FILE, X_OK)) != ERRC_NOERROR)
			return (NULL);
		else
			path_exec = ft_strdup(name);
	}
	if (path_exec == NULL && *err == ERRC_NOERROR)
		*err = ERRC_UNEXPECTED;
	return (path_exec);
}

void				execute_cmd(t_alloc *alloc, char **argv, char *path_exec)
{
	char	**tab_env;

	tab_env = get_environ_from_list(alloc->vars);
	if (tab_env == NULL)
		exit(127);
	sig_reset();
	execve(path_exec, argv, tab_env);
	ft_dprintf(STDERR_FILENO, "42sh: %s: not executable\n", argv[0]);
	ft_strdel(&path_exec);
	ft_strtab_free(tab_env);
	exit(126);
}

int					exec_input(t_alloc *alloc, t_ast *elem, t_exec_opt *opt)
{
	t_error	err;
	int		hashable;
	char	*path_exec;
	pid_t	child;
	int		ret;

	hashable = 0;
	if ((path_exec = exec_path(alloc, elem->input[0], &hashable, &err)) == NULL)
	{
		exec_file_error(err, elem->input[0]);
		return (err == ERRC_FILENOTFOUND ? 127 : 126);
	}
	if (hashable == 1)
		set_exec_path(alloc->exectable, elem->input[0], path_exec, 1);
	if ((child = (!opt->fork) ? fork() : 0) == -1)
		return (1);
	else if (child == 0 && (opt->fork = 1) == 1)
	{
		if (elem->left != NULL && (ret = analyzer(alloc, elem->left, opt)) != 0)
			exit(ret);
		execute_cmd(alloc, elem->input, path_exec);
	}
	set_signals_handlers();
	ft_strdel(&path_exec);
	wait_pid(child, alloc, elem, opt);
	if (opt->wait_hang == 0)
		return (ret_status(alloc->ret_val, child, 0));
	return (0);
}
