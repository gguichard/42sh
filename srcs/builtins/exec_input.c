#include "shell.h"
#include "builtins.h"
#include "error.h"
#include "exectable.h"
#include "search_exec.h"
#include "check_path.h"
#include "hashtable.h"

static char	*srch_exec(t_var *lst_env, t_ast *elem, int *hashable)
{
	char	*s;
	t_error	error;

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

static char	*exec_path(t_ast *elem, t_alloc *alloc, int *hashable)
{
	const char	*alias;
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
	else if ((alias = get_exec_path(alloc->exectable, elem->input[0], 1)))
		path_exec = ft_strdup(check_right_alias((char *)alias));
	else
		path_exec = srch_exec(*(alloc->var), elem, hashable);
	return (path_exec);
}

static void	execute_cmd(char *path_exec, t_ast *elem, char **tab_env)
{
	execve(path_exec, elem->input, tab_env);
	ft_dprintf(2, "42sh: %s: not executable\n", elem->input[0]);
	exit(126);
}

void	redirect_term_controller(pid_t new_controler, int type)
{
	if (type == 0)
		tcsetpgrp(STDIN_FILENO, getpgid(new_controler));
	else if (type == 1)
	{
		signal(SIGTTOU, SIG_IGN);
		tcsetpgrp(STDIN_FILENO, getpgid(0));
		signal(SIGTTOU, SIG_DFL);
	}
}

int			exec_input(t_ast *elem, t_alloc *alloc, t_exec_opt *opt)
{
	pid_t	child;
	int		hashable;
	char	**tab_env;
	char	*path_exec;

	hashable = 0;
	child = 0;
	if (!(path_exec = exec_path(elem, alloc, &hashable)))
		return (127);
	convert_lst_tab(*(alloc->var), &tab_env);
	if (opt->fork == true || !(child = fork()))
		execute_cmd(path_exec, elem, tab_env);
	if (child == -1)
		return (0);
	setpgid(child, 0);
	add_pid_lst(child, elem);
	if (opt->wait_hang == false)
	{
		redirect_term_controller(child, 0);
		waitpid(child, &alloc->ret_val, WUNTRACED);
		redirect_term_controller(0, 1);
	}
	else
		waitpid(child, &alloc->ret_val, WNOHANG);
	if (hashable == 1)
		set_exec_path(alloc->exectable, elem->input[0], path_exec, 1);
	delete_str_tab(tab_env);
	if (!ft_strchr(elem->input[0], '/'))
		ft_memdel((void **)&path_exec);
	return (ret_status(alloc->ret_val, child));
}
