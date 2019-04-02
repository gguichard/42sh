#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"

int		initialize_inhib_expand(t_str_cmd_inf **str_cmd, char ***array,
		const char *str)
{
	if (!(*array = (char **)malloc(sizeof(char *) * 2)))
		return (0);
	if (!(*str_cmd = (t_str_cmd_inf*)malloc(sizeof(t_str_cmd_inf))))
		return (0);
	if (!scmd_init(*str_cmd, str))
		return (0);
	if (!((*array)[0] = ft_strdup((*str_cmd)->str)))
		return (0);
	(*array)[1] = NULL;
	return (1);
}

int		go_to_end_quote(t_str_cmd_inf *str_cmd, char **array, size_t *pos)
{
	size_t	i;

	i = get_pos_in_array(array);
	remove_escaped_char(str_cmd, &(array[i]), pos, 0);
	*pos -= 1;
	while (scmd_cur_char(str_cmd) && str_cmd->is_in_quote == 1)
		*pos += scmd_move_to_next_char(str_cmd);
	str_cmd->pos -= 1;
	if (scmd_cur_char(str_cmd) == '\''
			|| (scmd_cur_char(str_cmd) == '\\' && !str_cmd->is_in_quote
			&& !str_cmd->is_in_dbquote))
	{
		str_cmd->pos += 1;
		remove_escaped_char(str_cmd, &(array[i]), pos, 0);
		*pos -= 1;
	}
	else
	{
		str_cmd->pos += 1;
		*pos += 1;
	}
	return (1);
}

int		inhib_expand_in_quote(t_str_cmd_inf *str_cmd, char **array,
		size_t *pos, t_alloc *alloc)
{
	if (str_cmd->is_in_quote)
		return (go_to_end_quote(str_cmd, array, pos));
	else if (str_cmd->is_in_dbquote)
		return (inhib_in_db(str_cmd, pos, array, alloc));
	return (1);
}
