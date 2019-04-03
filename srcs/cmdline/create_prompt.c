#include "libft.h"
#include "vars.h"
#include "cmdline.h"

char	*create_prompt(t_list *vars)
{
	const char	*ps1;

	ps1 = get_var_value(vars, "PS1");
	return (ft_strdup(ps1));
}
