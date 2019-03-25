#include "shell.h"
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

void				set_alloc(t_alloc *al, t_var **lst)
{
	int	idx;

	al->var = lst;
	idx = 0;
	while (idx < 10)
		al->fd[idx++] = -1;
	al->builtins = g_builtins;
	if ((al->exectable = make_exectable()) == NULL)
		ft_exit_malloc();
	if ((al->aliastable = make_def_hashtable()) == NULL)
		ft_exit_malloc();
}
