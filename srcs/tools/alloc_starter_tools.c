#include "shell.h"
#include "builtins.h"
#include "error.h"
#include "exectable.h"

static t_builtin	*set_tab_builtins(void)
{
	t_builtin			*builtins;
	static char			*lst_built[] = {"echo", "cd", "exit", "type", "hash",
		"alias", "unalias", "set", "unset", "export", "jobs", "fg", "bg"};
	static t_built_fun	lst_function[] = { &echo_builtins, &cd_builtins,
		&exit_builtins, &type_builtins, &hash_builtins, &alias_builtins,
		&unalias_builtins, &set_builtins, &unset_builtins, &export_builtins,
		&job_builtins, &fg_builtins, &bg_builtins };
	int					i;

	i = 0;
	if (!(builtins = (t_builtin *)malloc(sizeof(t_builtin) * 14)))
		ft_exit_malloc();
	while (i < 13)
	{
		builtins[i].name = lst_built[i];
		builtins[i].built_fun = lst_function[i];
		i += 1;
	}
	builtins[i].name = NULL;
	builtins[i].built_fun = NULL;
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
