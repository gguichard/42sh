#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"

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

//Permet d'inhiber d'expandre et de remove le quote
//Return 0 en cas d'erreur;
int	inhib_in_db(t_str_cmd_inf *str_cmd, size_t *pos, char **input,
		t_alloc *alloc)
{
	remove_escaped_char(str_cmd, input, pos);
	str_cmd->pos -= 1;
	*pos -= 1;
	while (scmd_cur_char(str_cmd) && str_cmd->is_in_dbquote)
	{
		if (scmd_cur_char_is_escaped(str_cmd) == 1
				&& scmd_cur_is_of(str_cmd, DBQUOTE_SPE_CHAR) == 1)
			remove_escaped_char(str_cmd, input, pos);
		else if (scmd_cur_char(str_cmd) == '$')
		{
			if (!expand(input, alloc, pos))
				return (0);
			scmd_move_to_next_char(str_cmd);
			update_pos_index(str_cmd);
		}
		else
		{
			scmd_move_to_next_char(str_cmd);
			*pos += 1;
		}
	}
	remove_escaped_char(str_cmd, input, pos);
	return (1);
}

//Inhib et expand une string et renvoi un tableau de string ou NULL en cas d'erreur
//si opt == 0 '\' en fin de ligne est delete
//si opt == 1 '\' fin de ligne reste (pour autocompletion)
char	**inhib_expand_str(const char *str, t_alloc *alloc, int opt)
{
	size_t			pos_array;
	t_str_cmd_inf	*str_cmd;
	char			**array;

	pos_array = 0;
	if (!(array = (char **)malloc(sizeof(char *) * 2)))
		return (NULL);
	if (!(str_cmd = (t_str_cmd_inf*)malloc(sizeof(t_str_cmd_inf))))
		return (NULL);
	if (!scmd_init(str_cmd, str))
		return (NULL);
	array[0] = ft_strdup(str_cmd->str);
	array[1] = NULL;
	while (scmd_cur_char(str_cmd))
	{
		if (str_cmd->is_in_quote || str_cmd->is_in_dbquote)
		{
			if (!(inhib_expand_in_quote(str_cmd, array, &pos_array, alloc)))
			{
				scmd_clean(str_cmd);
				delete_str_tab(array);
				return (NULL);
			}
		}
		else if (scmd_cur_char_is_escaped(str_cmd) && opt == 0)
			remove_escaped_char(str_cmd, &array[get_pos_in_array(array)],
					&pos_array);
		else if (scmd_cur_char_is_escaped(str_cmd) && opt == 1)
			remove_escaped_char_autocomplete(str_cmd,
					&array[get_pos_in_array(array)], &pos_array);
		else if (scmd_cur_char(str_cmd) == '$')
		{
			if (!expand(&(array[get_pos_in_array(array)]), alloc, &pos_array))
			{
				scmd_clean(str_cmd);
				delete_str_tab(array);
				return (NULL);
			}
			scmd_move_to_next_char(str_cmd);
			update_pos_index(str_cmd);
		}
		else
		{
			scmd_move_to_next_char(str_cmd);
			pos_array += 1;
		}
	}
	scmd_clean(str_cmd);
	return (array);
}

//Inhib et expand chaque string du tableau contenu dans l'AST
//Return 1 en cas de success et 0 e cas d'erreur
int		inhib_expand_tab(t_ast *elem, t_alloc *alloc)
{
	int		i;
	char	**new_array;

	i = 0;
	while (elem->input[i])
	{
		if (!(new_array = inhib_expand_str(elem->input[i], alloc, 0)))
			return (0);
		create_new_input(elem, &i, new_array);
		int x = 0;
		while (elem->input[x++])
			ft_printf("elem[%d]: %s\n", x, elem->input[x]);
		ft_printf("\n\n");
	}
	return (1);
}
