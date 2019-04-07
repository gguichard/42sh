#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "shell.h"
#include "vars.h"
#include "builtins.h"
#include "error.h"
#include "exectable.h"

static const t_builtin	g_builtins[] = {
	{"echo", builtin_echo},
	{"cd", builtin_cd},
	{"exit", builtin_exit},
	{"type", builtin_type},
	{"hash", builtin_hash},
	{"alias", builtin_alias},
	{"unalias", builtin_unalias},
	{"set", builtin_set},
	{"unset", builtin_unset},
	{"export", builtin_export},
	{"jobs", builtin_jobs},
	{"fg", builtin_fg},
	{"bg", builtin_bg},
	{"test", builtin_test},
	{"exec", builtin_exec},
	{"source", builtin_source},
	{".", builtin_source},
	{NULL, NULL}
};

static void				increase_shlvl(t_alloc *alloc)
{
	t_var	*var;
	long	shlvl;
	char	*endptr;
	char	*tmp;

	var = get_var(alloc->vars, "SHLVL");
	if (var == NULL)
		shlvl = 0;
	else
	{
		shlvl = ft_strtol(var->value, &endptr, 10);
		if (*endptr != '\0' || shlvl < 0 || shlvl >= INT_MAX)
			shlvl = 0;
	}
	tmp = ft_itoa((int)shlvl + 1);
	if (tmp != NULL)
	{
		update_var(&alloc->vars, "SHLVL", tmp);
		free(tmp);
	}
}

void					setup_def_vars(t_alloc *alloc)
{
	char	*ppid;
	char	*cur_pwd;

	increase_shlvl(alloc);
	unset_var(&alloc->vars, "PPID");
	ppid = ft_ulltoa(getppid());
	if (ppid != NULL)
	{
		create_var(&alloc->vars, "PPID", ppid, 0);
		free(ppid);
	}
	if (get_var(alloc->vars, "PWD") == NULL)
	{
		cur_pwd = getcwd(NULL, 0);
		if (cur_pwd != NULL)
		{
			create_var(&alloc->vars, "PWD", cur_pwd, 1);
			free(cur_pwd);
		}
	}
	if (get_var(alloc->vars, "_") == NULL)
		create_var(&alloc->vars, "_", alloc->argv[0], 1);
	if (get_var(alloc->vars, "PS1") == NULL)
		create_var(&alloc->vars, "PS1", "> ", 0);
}

int						setup_alloc(t_alloc *alloc, int argc, char **argv
		, char **environ)
{
	ft_memset(alloc, 0, sizeof(t_alloc));
	alloc->argc = argc;
	alloc->argv = argv;
	alloc->exit_rdy = 1;
	alloc->ppid = getpid();
	alloc->vars = parse_env(environ);
	setup_def_vars(alloc);
	alloc->builtins = g_builtins;
	alloc->exectable = make_exectable();
	if (alloc->exectable == NULL)
		return (0);
	alloc->aliastable = make_def_hashtable();
	if (alloc->aliastable == NULL)
		return (0);
	return (1);
}
