#include "shell.h"
#include "builtins.h"
#include "error.h"
#include "exectable.h"

static t_builtin	*set_tab_builtins(void)
{
	t_builtin			*builtins;
	static char			*lst_built[] = {
		"echo", "cd", "exit", "type", "hash"
			, "alias", "unalias", "set", "unset", "export", "jobs", "fg", "bg"
			, "test"
	};
	static t_built_fun	lst_function[] = {
		&echo_builtins, &cd_builtins, &exit_builtins, &type_builtins
			, &hash_builtins, &alias_builtins, &unalias_builtins, &set_builtins
			, &unset_builtins, &export_builtins, &job_builtins, &fg_builtins
			, &bg_builtins, &test_builtins };
	int					idx;

	idx = 0;
	if (!(builtins = (t_builtin *)malloc(sizeof(t_builtin) * 15)))
		ft_exit_malloc();
	while (idx < 14)
	{
		builtins[idx].name = lst_built[idx];
		builtins[idx].built_fun = lst_function[idx];
		idx += 1;
	}
	builtins[idx].name = NULL;
	builtins[idx].built_fun = NULL;
	return (builtins);
}

void				set_alloc(t_alloc *al, t_var **lst)
{
	int		x;

	al->var = lst;
	x = 0;
	while (x < 10)
		al->fd[x++] = -1;
	al->builtins = set_tab_builtins();
	if ((al->exectable = make_exectable()) == NULL)
		ft_exit_malloc();
	if ((al->aliastable = make_def_hashtable()) == NULL)
		ft_exit_malloc();
}
