#include "shell.h"
#include "builtins.h"
#include "error.h"
#include "search_exec.h"
#include "check_path.h"
#include "hashtable.h"

static char	*srch_exec(t_var *lst_env, t_ast *elem, int *hashable)
{
	char	*s;
	t_error	error;

	s = 0;
	s = search_exec(lst_env, elem->input[0], &error);
	if (error != ERRC_NOERROR)
	{
		ft_memdel((void **)&s);
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

static char	*exec_path(t_ast *elem, t_var *lst_env, t_alloc *alloc, int *hashable)
{
	t_hashentry	*alias;
	char		*path_exec;
	t_error		error;

	path_exec = 0;
	alias = 0;
	if (ft_strchr(elem->input[0], '/'))
	{
		if ((error = check_file_rights(elem->input[0], FT_FILE, X_OK)) != ERRC_NOERROR)
			exec_file_error(error, elem->input[0]);
		else
			path_exec = elem->input[0];
	}
	else if ((alias = get_hashentry(alloc->exectable, elem->input[0])))
		path_exec = check_right_alias((char *)alias->value);
	else
		path_exec = srch_exec(lst_env, elem, hashable);
	return (path_exec);
}

int			exec_input(t_ast *elem, t_var *lst_env, t_alloc *alloc)
{
	pid_t	child;
	int		hashable;
	char	**tab_env;
	char	*path_exec;

	hashable = 0;
	if (!(path_exec = exec_path(elem, lst_env, alloc, &hashable)))
		return (ret_status());
	convert_lst_tab(lst_env, &tab_env);
	if (!(child = fork()))
	{
		if (g_pid == -1)
			exit(130);
		g_in_exec = 1;
		execve(path_exec, elem->input, tab_env);
		ft_dprintf(2, "42sh: %s: not executable\n", elem->input[0]);
		exit(126);
	}
	g_pid = child;
	waitpid(child, &(g_ret[0]), 0);
	g_ret[1] = 1;
	if (hashable == 1)
		add_hashentry(alloc->exectable, elem->input[0], path_exec, sizeof(path_exec));
	delete_str_tab(tab_env);
	if (ft_strcmp(path_exec, elem->input[0]))
		ft_memdel((void **)&path_exec);
	return (ret_status());
}
