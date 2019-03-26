#include "shell.h"
#include "parser_lexer.h"

// ADD GESTION ERROR EXPAND

char	**insert_new_tab(char **modify, int *i, char **new, t_ast *elem)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (x < *i)
	{
		if (!(modify[x] = ft_strdup(elem->input[x])))
			ft_exit_malloc();
		x += 1;
	}
	while (new[y])
		if (!(modify[x++] = ft_strdup(new[y++])))
			ft_exit_malloc();
	y = *i + 1
	while (elem->input[y])
		if (!(modify[x++] = ft_strdup(elem->input[y++])))
			ft_exit_malloc();
	modify[x] = NULL;
	return (modify);
}

void	create_new_input(t_ast *elem, int *i, char **new)
{
	int		len_new;
	int		len_input;
	char	**modify;

	len_new = 0;
	len_input = 0;
	modify = NULL;
	while (new[len_new])
		len_new += 1;
	while (elem->input[len_input])
		len_input += 1;
	if (!(modify = (char**)malloc(sizeof(char*) * (len_input + len_new))))
		ft_exit_malloc();
	modify = insert_new_tab(modify, i, new, elem);
	ft_strtab_free(elem->input);
	elem->input = modify;
	*i += len_new;
}

void	expand(t_ast *elem, t_alloc *alloc)
{
	char		**new;
	const char	*str;
	int			i;

	new = NULL;
	i = 0;
	while (elem->input[i])
	{
		if (ft_strncmp(elem->input[i], "${", 2) == 0)
		{
			str = get_env_value(*(alloc->vars), &(elem->input[i][2]));
			if (ft_strcmp(str, "") == 0)
			new = ft_strsplit(str, " ");
			create_new_input(elem, &i, new);
		}
		i += 1;
	}
}
