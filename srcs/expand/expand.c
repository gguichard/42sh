#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"

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
	y = *i + 1;
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
	delete_str_tab(elem->input);
	elem->input = modify;
	*i += len_new;
}

int	expand(t_ast *elem, t_alloc *alloc)
{
	char		**new;
	char		*str;
	const char	*exp;
	int			i;
	int			x;

	new = NULL;
	exp = NULL;
	str = NULL;
	i = 0;
	x = 0;
	while (elem->input[i])
	{
		while ((exp = ft_strchr(&(elem->input[i][x]), '$')))
		{
			x = ft_strlen(elem->input[i]) - ft_strlen(exp);
			if (ft_strncmp(exp, "${", 2) == 0)
			{
				if (check_expand_syntax(&(exp[2])) == 0)
					return (error_expand(elem->input[i]));
				else
				{
					str = get_expand_value(*(alloc->var), &(exp[2]), 1);
					insert_var_input(str, &(elem->input[i]), 1);
				}
			}
			else if (check_expand_syntax(&(exp[1])) == 0)
				return (error_expand(elem->input[i]));
			else
			{
				str = get_expand_value(*(alloc->var), &(exp[1]), 0);
				insert_var_input(str, &(elem->input[i]), 0);
			}
		}
		if (str)
		{
			new = ft_strsplit(elem->input[i], ' ');
			create_new_input(elem, &i, new);
		}
		else
		  i += 1;
		str = NULL;
		x = 0;
	}
	return (1);
}
