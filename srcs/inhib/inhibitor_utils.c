#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"

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
	str_cmd->pos -= 1;
	while (scmd_cur_char(str_cmd) && str_cmd->is_in_quote == 1)
	{
		scmd_move_to_next_char(str_cmd);
		*pos += 1;
	}
	str_cmd->pos -= 1;
	if (scmd_cur_char(str_cmd) == '\'')
	{
		str_cmd->pos += 1;
		*pos -= 1;
		remove_escaped_char(str_cmd, input, pos);
	}
	else
		str_cmd->pos += 1;
	return (1);
}

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
