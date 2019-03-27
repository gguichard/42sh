#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"

char		**insert_new_tab(char **modify, int *i, char **new, t_ast *elem)
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

void		create_new_input(t_ast *elem, int *i, char **new)
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

int			expand_var(char **str, t_alloc *alloc, const char *exp)
{
	if (ft_strncmp(exp, "${", 2) == 0)
			*str = get_expand_value(&(exp[2]), 1, alloc);
	else
		*str = get_expand_value(&(exp[1]), 0, alloc);
	return (1);
}

int			expand(char **input, t_alloc *alloc, size_t *pos)
{
	char		*str;
	const char	*exp;

	exp = NULL;
	str = NULL;
	exp = ft_strchr(*input + *pos, '$');
	expand_var(&str, alloc, exp);
	if ((*input)[*pos + 1] == '{' && str)
		insert_var_input(str, input, 1, *pos);
	else
		insert_var_input(str, input, 0, *pos);
	if (str)
	{
		*pos += ft_strlen(str);
		ft_memdel((void **)&str);
	}
	ft_printf("INPUT: |%s|\n", *input);
	return (1);
}
