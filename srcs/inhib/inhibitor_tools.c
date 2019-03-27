#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"

void	remove_escaped_char(t_str_cmd_inf *str_cmd, char **input, size_t *pos_elem)
{
	size_t	len;

	if (*pos_elem == 0)
		*pos_elem = str_cmd->pos;
	len = ft_strlen(*input + *pos_elem);
	ft_memmove((void*)(*input + *pos_elem - 1),
		(const void*)(*input + *pos_elem), len);
	(*input)[*pos_elem + len - 1] = '\0';
	if (scmd_cur_char(str_cmd) == '\n')
	{
		len = ft_strlen(*input + *pos_elem);
		ft_memmove((void*)(*input + *pos_elem - 1),
			(const void*)(*input + *pos_elem), len);
		(*input)[*pos_elem + len - 1] = '\0';
		if (*pos_elem > 0)
			*pos_elem -= 1;
	}
	scmd_move_to_next_char(str_cmd);

}

void	remove_escaped_char_quote(t_str_cmd_inf *str_cmd, char **input,
		size_t *pos_elem)
{
	size_t	len;

	if (*pos_elem == 0)
		*pos_elem = str_cmd->pos;
	len = ft_strlen(*input + *pos_elem);
	ft_memmove((void*)(*input + *pos_elem - 1),
		(const void*)(*input + *pos_elem), len);
	(*input)[*pos_elem + len - 1] = '\0';
	if (*pos_elem > 0)
		*pos_elem -= 1;
}

void	go_to_end_quote(t_str_cmd_inf *str_cmd, char **input, size_t *pos_elem)
{
	remove_escaped_char(str_cmd, input, pos_elem);
	while (str_cmd->is_in_quote == 1)
	{
		scmd_move_to_next_char(str_cmd);
		*pos_elem += 1;
	}
	remove_escaped_char(str_cmd, input, pos_elem);
}

void	update_pos_index(t_str_cmd_inf *str_cmd, size_t *pos_elem)
{
	(void)pos_elem;
	if (!ft_isdigit(scmd_cur_char(str_cmd)))
	{
		while (scmd_cur_is_of(str_cmd, DBQUOTE_SPE_CHAR) == 0
				&& scmd_cur_char(str_cmd)
				&& ft_isalnum(scmd_cur_char(str_cmd)))
			scmd_move_to_next_char(str_cmd);
	}
	else
		scmd_move_to_next_char(str_cmd);
}

void	update_pos_index_in_db(t_str_cmd_inf *str_cmd, size_t *pos_elem)
{
	(void)pos_elem;
	if (!ft_isdigit(scmd_cur_char(str_cmd)))
	{
		while (scmd_cur_is_of(str_cmd, DBQUOTE_SPE_CHAR) == 0
				&& scmd_cur_char(str_cmd)
				&& ft_isalnum(scmd_cur_char(str_cmd)))
			scmd_move_to_next_char(str_cmd);
	}
	else
		scmd_move_to_next_char(str_cmd);
}
