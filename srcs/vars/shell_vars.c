#include "shell.h"
#include "vars.h"

const char	*get_var_value(t_list *vars, const char *key)
{
	t_var	*var;

	var = get_var(vars, key);
	return (var == NULL ? "" : var->value);
}

char		*get_var_for_expand(t_alloc *alloc, const char *key)
{
	t_var	*var;
	int		tmp;

	var = NULL;
	if (key == NULL)
		return (0);
	if (ft_isalpha(key[0]) || key[0] == '_')
		var = get_var(alloc->vars, key);
	else
	{
		if (ft_strequ(key, "?"))
			return (ft_itoa(alloc->ret_val));
		if (ft_strequ(key, "!") && alloc->last_bg)
			return (ft_itoa(alloc->last_bg));
		if (ft_strequ(key, "$"))
			return (ft_itoa(alloc->ppid));
		if (ft_isdigit(key[0]))
		{
			tmp = ft_atoi(key);
			return (ft_strdup(alloc->argc <= tmp ? "" : alloc->argv[tmp]));
		}
	}
	if (var == NULL || var->is_env == 2)
		return (ft_strdup(""));
	else
		return (ft_strdup(var->tmp_value != NULL ? var->tmp_value : var->value));
}
