#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"

void	remove_escaped_char(t_str_cmd_inf *str_cmd, char **input, size_t *pos,
		int next)
{
	size_t	len;

	if (*pos == 0)
		*pos = str_cmd->pos;
	len = ft_strlen(*input + *pos);
	ft_memmove((void*)(*input + *pos - 1),
		(const void*)(*input + *pos), len);
	(*input)[*pos + len - 1] = '\0';
	if (scmd_cur_char_is_escaped(str_cmd) == 1
			&& scmd_cur_char(str_cmd) == '\n' && str_cmd->is_in_dbquote)
	{
		len = ft_strlen(*input + *pos);
		ft_memmove((void*)(*input + *pos - 1),
			(const void*)(*input + *pos), len);
		(*input)[*pos + len - 1] = '\0';
		if (*pos > 0)
			*pos -= 1;
	}
	if (next == 1)
		scmd_move_to_next_char(str_cmd);
}

void	remove_last_char(t_str_cmd_inf *str_cmd, size_t *pos, char **input)
{
	str_cmd->pos -= 1;
	*pos -= 1;
	if ((scmd_cur_char(str_cmd) == '"' && (*input)[*pos] == '"'
			&& !scmd_cur_char_is_escaped(str_cmd))
			|| (scmd_cur_char(str_cmd) == '\'' && (*input)[*pos] == '\''
			&& !scmd_cur_char_is_escaped(str_cmd))
			|| (scmd_cur_char(str_cmd) == '\\'
			&& !scmd_cur_char_is_escaped(str_cmd) && !str_cmd->is_in_quote
			&& !str_cmd->is_in_dbquote))
	{
		*pos += 1;
		remove_escaped_char(str_cmd, input, pos, 1);
		*pos -= 1;
	}
	str_cmd->pos += 1;
}

void	remove_last_quote(t_str_cmd_inf *str_cmd, char **array, size_t *pos,
		size_t index)
{
	str_cmd->pos -= 1;
	if ((scmd_cur_char(str_cmd) == '\"'
			&& !scmd_cur_char_is_escaped(str_cmd))
			|| scmd_cur_char(str_cmd) == '\\')
	{
		str_cmd->pos += 1;
		remove_escaped_char(str_cmd, &(array[index]), pos, 0);
		*pos -= 1;
	}
	else
	{
		str_cmd->pos += 1;
		*pos += 1;
	}
}
