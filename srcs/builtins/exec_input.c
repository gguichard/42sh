#include "libft.h"
#include "shell.h"
#include "vars.h"
#include "builtins.h"
#include "error.h"
#include "exectable.h"
#include "search_exec.h"
#include "check_path.h"
#include "hashtable.h"

static char	*srch_exec(t_list *vars, t_ast *elem, int *hashable)
{
	char	*s;
	t_error	error;

	s = search_exec(vars, elem->input[0], &error);
	if (error != ERRC_NOERROR)
	{
		ft_strdel(&s);
		exec_file_error(error, elem->input[0]);
	}
	*hashable = 1;
	return (s);
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

	path_exec = 0;
	alias = 0;
	if (ft_strchr(elem->input[0], '/'))
	{
		if ((error = check_file_rights(elem->input[0], FT_FILE, X_OK))
				!= ERRC_NOERROR)
			exec_file_error(error, elem->input[0]);
		else
			path_exec = elem->input[0];
	}
	else if ((alias = get_exec_path(alloc->exectable, elem->input[0], 1)))
		path_exec = ft_strdup(check_right_alias((char *)alias));
	else
		path_exec = srch_exec(alloc->vars, elem, hashable);
	return (path_exec);
}

static void	execute_cmd(char *path_exec, t_ast *elem, char **tab_env
		, int no_fork)
{
	if (g_pid == -1)
		exit(130);
	g_in_exec = 1;
	execve(path_exec, elem->input, tab_env);
	ft_dprintf(2, "42sh: %s: not executable\n", elem->input[0]);
	if (no_fork == 1)
	{
		ft_strtab_free(tab_env);
		if (ft_strcmp(path_exec, elem->input[0]))
			ft_memdel((void **)&path_exec);
	}
	exit(126);
}

int			exec_input(t_ast *elem, t_alloc *alloc, int no_fork)
{
	pid_t	child;
	int		hashable;
	char	**tab_env;
	char	*path_exec;

	hashable = 0;
	child = 0;
	if (!(path_exec = exec_path(elem, alloc, &hashable)))
		return (ret_status());
	tab_env = get_environ_from_list(alloc->vars); // TODO: check if NULL?
	if (no_fork == 1 || !(child = fork()))
		execute_cmd(path_exec, elem, tab_env, no_fork);
	if (child == -1)
		return (0);
	g_pid = child;
	waitpid(child, &(g_ret[0]), 0);
	g_ret[1] = 1;
	if (hashable == 1)
		set_exec_path(alloc->exectable, elem->input[0], path_exec, 1);
	ft_strtab_free(tab_env);
	if (!ft_strchr(elem->input[0], '/'))
		ft_memdel((void **)&path_exec);
	return (ret_status());
}
