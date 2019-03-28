#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"

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

void	remove_escaped_char(t_str_cmd_inf *str_cmd, char **input, size_t *pos)
{
	size_t	len;

	if (*pos == 0)
		*pos = str_cmd->pos;
	len = ft_strlen(*input + *pos);
	ft_memmove((void*)(*input + *pos - 1),
		(const void*)(*input + *pos), len);
	(*input)[*pos + len - 1] = '\0';
	if (scmd_cur_char(str_cmd) == '\n' && str_cmd->is_in_dbquote)
	{
		len = ft_strlen(*input + *pos);
		ft_memmove((void*)(*input + *pos - 1),
			(const void*)(*input + *pos), len);
		(*input)[*pos + len - 1] = '\0';
		if (*pos > 0)
			*pos -= 1;
	}
	scmd_move_to_next_char(str_cmd);
}

//FONCTION D'ECHAPPEMENT POUR L'AUTOCOMPLETION
//VERIFER SI L"ON SUPPRIME PAS QUE SUR \0
void	remove_escaped_char_autocomplete(t_str_cmd_inf *str_cmd, char **input,
		size_t *pos)
{
	size_t	len;

	if (*pos == 0)
		*pos = str_cmd->pos;
	if (scmd_cur_char(str_cmd) != '\0')
	{
		len = ft_strlen(*input + *pos);
		ft_memmove((void*)(*input + *pos - 1),
			(const void*)(*input + *pos), len);
		(*input)[*pos + len - 1] = '\0';
		if (*pos > 0)
			*pos -= 1;
	}
	scmd_move_to_next_char(str_cmd);
}

int		go_to_end_quote(t_str_cmd_inf *str_cmd, char **input, size_t *pos)
{
	remove_escaped_char(str_cmd, input, pos);
	while (str_cmd->is_in_quote == 1)
	{
		scmd_move_to_next_char(str_cmd);
		*pos += 1;
	}
	remove_escaped_char(str_cmd, input, pos);
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
		// str_cmd->pos -= 1;
	}
}
