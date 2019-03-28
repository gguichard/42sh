#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"

int	inhib_in_db(t_str_cmd_inf *str_cmd, size_t *pos_elem, char **input,
		t_alloc *alloc)
{
	remove_escaped_char_quote(str_cmd, input, pos_elem);
	while (scmd_cur_char(str_cmd) && str_cmd->is_in_dbquote)
	{
		if (scmd_cur_char_is_escaped(str_cmd) == 1
				&& scmd_cur_is_of(str_cmd, DBQUOTE_SPE_CHAR) == 1)
			remove_escaped_char(str_cmd, input, pos_elem);
		else if (scmd_cur_char(str_cmd) == '$')
		{
			if (!expand(input, alloc, pos_elem))
				return (0);
			scmd_move_to_next_char(str_cmd);
			update_pos_index(str_cmd);
		}
		else
		{
			scmd_move_to_next_char(str_cmd);
			*pos_elem += 1;
		}
	}
	remove_escaped_char(str_cmd, input, pos_elem);
	return (1);
}

int	inhib_quote(t_str_cmd_inf *str_cmd, size_t *pos_elem,
		char **input, t_alloc *alloc)
{
	if (str_cmd->is_in_quote)
		return (go_to_end_quote(str_cmd, input, pos_elem));
	else if (str_cmd->is_in_dbquote)
		return (inhib_in_db(str_cmd, pos_elem, input, alloc));
	return (1);
}

int		inhib_all(t_str_cmd_inf *str_cmd, char **str, t_alloc *alloc)
{
	size_t	pos_elem;

	pos_elem = 0;
	while (scmd_cur_char(str_cmd))
		if (str_cmd->is_in_quote || str_cmd->is_in_dbquote)
		{
			if (!inhib_quote(str_cmd, &pos_elem, str, alloc))
				return (0);
		}
		else if (scmd_cur_char_is_escaped(str_cmd) == 1)
			remove_escaped_char(str_cmd, str, &pos_elem);
		else if (scmd_cur_char(str_cmd) == '$')
		{
			if (!expand(str, alloc, &pos_elem))
				return (0);
			scmd_move_to_next_char(str_cmd);
			update_pos_index(str_cmd);
		}
		else
		{
			scmd_move_to_next_char(str_cmd);
			pos_elem += 1;
		}
	return (1);
}

int		inhibitor(t_ast *elem, t_alloc *alloc)
{
	t_str_cmd_inf	*str_cmd;
	int				i;

	i = 0;
	str_cmd = NULL;
	if (!(str_cmd = (t_str_cmd_inf*)malloc(sizeof(t_str_cmd_inf))))
		ft_exit_malloc();
	while (elem->input[i])
	{
		if (!scmd_init(str_cmd, elem->input[i]))
			return (0);
		if (!inhib_all(str_cmd, &(elem->input[i]), alloc))
		{
			scmd_clean(str_cmd);
			return (0);
		}
		scmd_clean(str_cmd);
		i += 1;
	}
	return (1);
}
