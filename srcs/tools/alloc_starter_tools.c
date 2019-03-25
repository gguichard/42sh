#include "shell.h"
#include "vars.h"
#include "builtins.h"
#include "error.h"
#include "exectable.h"

static const t_builtin	g_builtins[] = {
	{"echo", builtin_echo},
	{"cd", cd_builtins},
	{"exit", builtin_exit},
	{"type", builtin_type},
	{"hash", builtin_hash},
	{"alias", builtin_alias},
	{"unalias", builtin_unalias},
	{"set", builtin_set},
	{"unset", builtin_unset},
	{"export", export_builtins},
	{"jobs", builtin_jobs},
	{"fg", builtin_fg},
	{"bg", builtin_bg},
	{"test", builtin_test},
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
	t_var	*var;
	char	*cur_pwd;

	increase_shlvl(alloc);
	var = get_var(alloc->vars, "PWD");
	if (var == NULL)
	{
		cur_pwd = getcwd(NULL, 0);
		if (cur_pwd != NULL)
		{
			create_var(&alloc->vars, "PWD", cur_pwd, 1);
			free(cur_pwd);
		}
	}
}

int						setup_alloc(t_alloc *alloc, int argc, char **argv
		, char **environ)
{
	int	idx;

	ft_memset(alloc, 0, sizeof(t_alloc));
	alloc->argc = argc;
	alloc->argv = argv;
	alloc->vars = parse_env(environ);
	setup_def_vars(alloc);
	idx = 0;
	while (idx < 10)
	{
		alloc->fd[idx] = -1;
		idx++;
	}
	alloc->builtins = g_builtins;
	if ((alloc->exectable = make_exectable()) == NULL)
		return (0);
	if ((alloc->aliastable = make_def_hashtable()) == NULL)
		return (0);
	return (1);
}
