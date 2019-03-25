#include <stdlib.h>
#include "libft.h"
#include "shell.h"
#include "builtins.h"

// BUILTINS UNSET POSIX NORME NO OPTIONS
// JUST UNSET ALL THE VARIABLES
// IF THERE IS AN '=' IN THE NAME VARIABLE
// RETURN ERROR

static int	is_valid_identifier(const char *key)
{
	size_t	offset;

	if (ft_isdigit(key[0]))
		return (0);
	offset = 0;
	while (key[offset] != '\0')
	{
		if (!ft_isalnum(key[offset]) && key[offset] != '_')
			return (0);
		offset++;
	}
	return (1);
}

static void	del_var_by_key(t_var **lst, const char *key)
{
	t_var	*prev;
	t_var	*tmp;

	prev = NULL;
	tmp = *lst;
	while (tmp != NULL)
	{
		if (ft_strequ(tmp->key, key))
		{
			if (prev == NULL)
				*lst = tmp->next;
			else
				prev = tmp->next;
			free(tmp->key);
			free(tmp->value);
			free(tmp);
			break ;
		}
		prev = tmp;
		tmp = tmp->next;
	}
}

int			builtin_unset(t_ast *elem, t_alloc *alloc)
{
	int	idx;
	int	ret;

	idx = 1;
	ret = 0;
	while (elem->input[idx] != NULL)
	{
		if (is_valid_identifier(elem->input[idx]))
			del_var_by_key(alloc->var, elem->input[idx]);
		else
		{
			ft_dprintf(2, "42sh: unset: %s: not a valid identifier"
					, elem->input[idx]);
			ret = 1;
		}
		idx++;
	}
	return (ret);
}
