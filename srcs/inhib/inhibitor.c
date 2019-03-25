#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"


void	go_to_end_quote(t_str_cmd_inf *str_cmd, t_ast *elem, int i,
		size_t *count_escape)
{
	size_t	pos;
	size_t	len;

	pos = str_cmd->pos - *count_escape;
	len = ft_strlen(&(elem->input[i][pos]));
	ft_memmove((void*)&(elem->input[i][pos - 1]),
		(const void*)&(elem->input[i][pos]), len);
	elem->input[i][len + pos - 1] = '\0';
	*count_escape += 1;
	while (str_cmd->is_in_quote == 1)
		scmd_move_to_next_char(str_cmd);
	pos = str_cmd->pos - *count_escape;
	len = ft_strlen(&(elem->input[i][pos]));
	ft_memmove((void*)&(elem->input[i][pos - 1]),
		(const void*)&(elem->input[i][pos]), len);
	elem->input[i][len + pos - 1] = '\0';
}

void	remove_escaped_char(t_str_cmd_inf *str_cmd, t_ast *elem, int i,
		size_t *count_escape)
{
	size_t	len;
	size_t	pos;

	pos = str_cmd->pos - *count_escape;
	// ft_printf("\n\nINT POS ELEM %d: |%s|\n", pos, &(elem->input[i][pos]));
	len = ft_strlen(&(elem->input[i][pos]));
	ft_memmove((void*)&(elem->input[i][pos - 1]),
		(const void*)&(elem->input[i][pos]), len);
	elem->input[i][pos + len - 1] = '\0';
	// scmd_clean(str_cmd);
	// scmd_init(str_cmd, elem->input[i]);
	str_cmd->pos = pos + 1;
	// ft_printf("elem at %d: |%s|\n", pos, &(elem->input[i][pos]));
	*count_escape += 1;
}

int	inhibitor(t_ast *elem)
{
	t_str_cmd_inf	*str_cmd;
	int				i;
	size_t			count_escape;

	i = 0;
	count_escape = 0;
	str_cmd = NULL;
	if (!(str_cmd = (t_str_cmd_inf*)malloc(sizeof(t_str_cmd_inf))))
		ft_exit_malloc();
	while (elem->input[i])
	{
		ft_printf("INPUT[%d]: |%s|\n", i, elem->input[i]);
		if (!scmd_init(str_cmd, elem->input[i]))
			return (0);
		while (elem->input[i][str_cmd->pos])
		{
			if (str_cmd->is_in_quote == 1)
				go_to_end_quote(str_cmd, elem, i, &count_escape);
			// else if (str_cmd->is_in_dbquote)
				// expand_and_desinhib(elem, i, str_cmd);
			else if (scmd_cur_char_is_escaped(str_cmd) == 1)
				remove_escaped_char(str_cmd, elem, i, &count_escape);
			scmd_move_to_next_char(str_cmd);
			ft_printf("input pos after desinhib: |%s|\n", &(str_cmd->str[str_cmd->pos]));
			// ft_printf("input[%d]: |%s| next pos = %d\n", i, elem->input[i], str_cmd->pos);
		}
		scmd_clean(str_cmd);
		// ft_printf("input[%d]: |%s|\n", i, elem->input[i]);
		i += 1;
	}
	return (1);
}
