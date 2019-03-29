#include <stdlib.h>
#include <unistd.h>
#include "libft.h"
#include "shell.h"
#include "vars.h"
#include "job.h"
#include "error.h"
#include "hashtable.h"
#include "exectable.h"
#include "search_exec.h"
#include "check_path.h"
#include "execution.h"
#include "builtins.h" // TODO: deplacer le prototype de wait_pid

static char	*srch_exec(t_list *vars, t_ast *elem, int *hashable)
{
	char	*path;
	t_error	error;

	path = search_exec(vars, elem->input[0], &error);
	if (error != ERRC_NOERROR)
	{
		ft_strdel(&path);
		exec_file_error(error, elem->input[0]);
	}
	*hashable = 1;
	return (path);
}

static char	*check_right_alias(char *alias)
{
	t_error	error;

	if ((error = check_file_rights(alias, FT_FILE, X_OK)) != ERRC_NOERROR)
	{
		exec_file_error(error, alias);
		return (0);
	}
	return (alias);
}

static char	*exec_path(t_ast *elem, t_alloc *alloc, int *hashable)
{
	const char	*alias;
	char		*path_exec;
	t_error		error;

	path_exec = NULL;
	alias = NULL;
	if (ft_strchr(elem->input[0], '/'))
	{
		if ((error = check_file_rights(elem->input[0], FT_FILE, X_OK))
				!= ERRC_NOERROR)
			exec_file_error(error, elem->input[0]);
		else
			path_exec = ft_strdup(elem->input[0]);
	}
	else if ((alias = get_exec_path(alloc->exectable, elem->input[0], 1))
			!= NULL)
		path_exec = ft_strdup(check_right_alias((char *)alias));
	else
		path_exec = srch_exec(alloc->vars, elem, hashable);
	return (path_exec);
}

void		execute_cmd(t_alloc *alloc, t_ast *elem, char *path_exec)
{
	char	**tab_env;

	tab_env = get_environ_from_list(alloc->vars);
	if (tab_env == NULL)
		exit(127);
	execve(path_exec, elem->input, tab_env);
	ft_dprintf(STDERR_FILENO, "42sh: %s: not executable\n", elem->input[0]);
	ft_strdel(&path_exec);
	ft_strtab_free(tab_env);
	exit(126);
}

int			exec_input(t_ast *elem, t_alloc *alloc, t_exec_opt *opt)
{
	int		hashable;
	char	*path_exec;
	pid_t	child;
	int		ret;

	hashable = 0;
	if ((path_exec = exec_path(elem, alloc, &hashable)) == NULL)
		return (127);
	if (hashable == 1)
		set_exec_path(alloc->exectable, elem->input[0], path_exec, 1);
	child = 0;
	if (!opt->fork)
		child = fork();
	opt->fork = 1;
	if (child == -1)
		return (1);
	else if (child == 0)
	{
		if (elem->left != NULL && (ret = analyzer(elem->left, alloc, opt)) != 0)
			exit(ret);
		execute_cmd(alloc, elem, path_exec);
	}
	opt->fork = 0;
	ft_strdel(&path_exec);
	wait_pid(child, elem, opt, alloc);
	return (ret_status(alloc->ret_val, child, 0));
}
