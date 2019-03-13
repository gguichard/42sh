#include "shell.h"
#include "builtins.h"
#include "error.h"

// BUILTINS UNSET POSIX NORME NO OPTIONS
// JUST UNSET ALL THE VARIABLES
// IF THERE IS AN '=' IN THE NAME VARIABLE
// RETURN ERROR

static void	del_elem_env(t_var *elem)
{
	ft_strdel(&(elem->key));
	ft_strdel(&(elem->value));
	ft_memdel((void **)&elem);
}

static int	del_elem_env_by_key(t_var **lst, const char *key)
{
	t_var	*tmp;
	t_var	*tmp_next;

	tmp = *lst;
	if (ft_strcmp(tmp->key, key) == 0)
	{
		*lst = tmp->next;
		del_elem_env(tmp);
		return (0);
	}
	tmp_next = tmp->next;
	while (tmp_next)
	{
		if (ft_strcmp(tmp_next->key, key) == 0)
		{
			tmp->next = tmp_next->next;
			del_elem_env(tmp_next);
			return (0);
		}
		tmp = tmp->next;
		tmp_next = tmp->next;
	}
	return (0);
}

int			unset_builtins(t_ast *elem, t_var *lst_env,t_alloc *alloc)
{
	int	i;
	int	ret;

	(void)alloc;
	i = 1;
	ret = 0;
	while (elem->input[i])
	{
		if (!ft_strchr(elem->input[i], '='))
			del_elem_env_by_key(&lst_env, elem->input[i]);
		else
			ret = error_unset(elem->input[i]);
		i += 1;
	}
	return (ret);
}
