#include "../../includes/shell.h"
#include "../../includes/builtins.h"

int	echo_builtins(t_ast *elem, t_var **lst_env, t_alloc **alloc)
{
	int i;
	int option;

	(void)lst_env;
	(void)alloc;
	if (!elem->input[1])
	{
		write(1, "\n", 1);
		g_ret[0] = 0;
		return (0);
	}
	option = (ft_strcmp(elem->input[1], "-n") == 0) ? 1 : 0;
	i = (option == 1 || ft_strcmp(elem->input[1], "-") == 0) ? 2 : 1;
	while (elem->input[i])
		if (ft_strcmp(elem->input[i], "") != 0)
		{
			write(1, elem->input[i], ft_strlen(elem->input[i]));
			i += 1;
			(elem->input[i]) ? write(1, " ", 1) : 0;
		}
		else
			i += 1;
	(option == 0) ? write(1, "\n", 1) : 0;
	g_ret[0] = 0;
	return (0);
}
