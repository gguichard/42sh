#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"

//Permet d'inhiber d'expandre et de remove le quote
//Return 0 en cas d'erreur;
int			inhib_in_db(t_str_cmd_inf *str_cmd, size_t *pos, char **input,
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
	str_cmd = NULL;
	array = NULL;
	if (!(initialize_inhib_expand(&str_cmd, &array, str)))
		return (error_inhib_expand(str_cmd, array));
	while (scmd_cur_char(str_cmd))
		if ((str_cmd->is_in_quote || str_cmd->is_in_dbquote)
				&& !(inhib_expand_in_quote(str_cmd, array, &pos_array, alloc)))
			return (error_inhib_expand(str_cmd, array));
		else if (scmd_cur_char_is_escaped(str_cmd) && opt == 0)
			remove_escaped_char_select(str_cmd, &array[get_pos_in_array(array)],
				&pos_array, opt);
		else if (scmd_cur_char(str_cmd) == '$'
				&& !do_expand(array, alloc, &pos_array, str_cmd))
			return (error_inhib_expand(str_cmd, array));
		else
			pos_array += scmd_move_to_next_char(str_cmd);
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
	}
	return (1);
}

int		inhib_expand_in_quote(t_str_cmd_inf *str_cmd, char **array,
		size_t *pos, t_alloc *alloc)
{
	size_t	i;

	i = get_pos_in_array(array);
	if (str_cmd->is_in_quote)
		return (go_to_end_quote(str_cmd, &array[i], pos));
	else if (str_cmd->is_in_dbquote)
			return (inhib_in_db(str_cmd, pos, &array[i], alloc));
	return (1);
}
