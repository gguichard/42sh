#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"

void	remove_escaped_char(t_str_cmd_inf *str_cmd, t_ast *elem, int i,
		size_t *pos_elem)
{
	size_t	len;

	if (*pos_elem == 0)
		*pos_elem = str_cmd->pos;
	len = ft_strlen(&(elem->input[i][*pos_elem]));
	ft_memmove((void*)&(elem->input[i][*pos_elem - 1]),
		(const void*)&(elem->input[i][*pos_elem]), len);
	elem->input[i][*pos_elem + len - 1] = '\0';
	*pos_elem -= 1;
}

void	go_to_end_quote(t_str_cmd_inf *str_cmd, t_ast *elem, int i,
		size_t *count_escape)
{
	remove_escaped_char(str_cmd, elem, i, count_escape);
	while (str_cmd->is_in_quote == 1)
		scmd_move_to_next_char(str_cmd);
	remove_escaped_char(str_cmd, elem, i, count_escape);
}

void	remove_escaped_char_db(t_ast *elem, int i, size_t *pos)
{
	size_t	len;

	len = ft_strlen(&(elem->input[i][*pos]));
	ft_memmove((void*)&(elem->input[i][*pos - 1]),
		(const void*)&(elem->input[i][*pos]), len);
	elem->input[i][*pos + len - 1] = '\0';
}
