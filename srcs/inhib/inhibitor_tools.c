#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"
#include "error.h"

size_t	get_pos_in_array(char **array)
{
	size_t	i;

	i = 0;
	if (!(*array))
		return (i);
	while (array[i])
		i += 1;
	return (i - 1);
}

void	update_pos_index(t_str_cmd_inf *str_cmd)
{
	if (scmd_cur_char(str_cmd) == '?' || scmd_cur_char(str_cmd) == '!'
			|| scmd_cur_char(str_cmd) == '$')
	{
		scmd_move_to_next_char(str_cmd);
		return ;
	}
	if (scmd_cur_char(str_cmd) != '{')
	{
		while (scmd_cur_is_of(str_cmd, DBQUOTE_SPE_CHAR) == 0
				&& scmd_cur_char(str_cmd)
				&& (ft_isalnum(scmd_cur_char(str_cmd))
				|| scmd_cur_char(str_cmd) == '_'))
			scmd_move_to_next_char(str_cmd);
	}
	else if (scmd_cur_char(str_cmd) == '{')
	{
		while (scmd_cur_is_of(str_cmd, DBQUOTE_SPE_CHAR) == 0
				&& scmd_cur_char(str_cmd)
				&& scmd_cur_char(str_cmd) != '}')
			scmd_move_to_next_char(str_cmd);
		scmd_move_to_next_char(str_cmd);
	}
}

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
	ft_strtab_free(elem->input);
	ft_strtab_free(new);
	elem->input = modify;
	*i += len_new;
}

int		error_inhib_expand(t_str_cmd_inf *str_cmd, char **array)
{
	scmd_clean(str_cmd);
	ft_strtab_free(array);
	return (0);
}
