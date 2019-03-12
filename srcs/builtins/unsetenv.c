#include "shell.h"
#include "builtins.h"
#include "error.h"

static int	check_unsetenv_error(t_ast *elem)
{
	if (!elem->input[1])
		return (error_unsetenv(2, NULL));
	else if (elem->input[2])
		return (error_unsetenv(1, NULL));
	return (0);
}

static int	unset_exist_env(t_ast *elem, t_var *lst_env)
{
	t_var	*tmp;
	t_var	*tmp_next;

	tmp = lst_env;
	tmp_next = tmp->next;
	while (tmp_next && ft_strcmp(elem->input[1], tmp_next->key) != 0)
	{
		tmp = tmp_next;
		tmp_next = tmp_next->next;
	}
	if (tmp_next)
	{
		tmp->next = tmp_next->next;
		free(tmp_next->key);
		free(tmp_next->value);
		free(tmp_next);
	}
	else
		return (error_unsetenv(3, elem->input[1]));
	return (0);
}

int			unsetenv_builtins(t_ast *elem, t_var **lst_env, t_alloc **alloc)
{
	t_var	*tmp;

	(void)alloc;
	tmp = *lst_env;
	if (check_unsetenv_error(elem) == -1)
		return (-1);
	else if (ft_strcmp(elem->input[1], tmp->key) == 0)
	{
		*lst_env = tmp->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
	else if (unset_exist_env(elem, *lst_env) == -1)
		return (-1);
	g_ret[0] = 0;
	return (0);
}
