#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"

int			expand_db(t_ast *elem, t_alloc *alloc, int *i, size_t *pos)
{
	char		**new;
	char		*str;
	const char	*exp;
	int			x;

	new = NULL;
	exp = NULL;
	str = NULL;
	x = 0;
	exp = ft_strchr(&(elem->input[*i][*pos]), '$');
	x = ft_strlen(elem->input[*i]) - ft_strlen(exp);
	if (expand_var(&str, alloc, exp, &(elem->input[*i])) == 0)
		return (0);
	if (elem->input[*i][*pos + 1] == '{')
		insert_var_input(str, &(elem->input[*i]), 1, *pos);
	else
		insert_var_input(str, &(elem->input[*i]), 0, *pos);
	return (ft_strlen(str));
}

void	inhib_in_dbquote(t_ast *elem, int i, t_str_cmd_inf **str_cmd,
		size_t *count_escape, t_alloc *alloc)
{
	size_t	pos;
	size_t	len;
	int		save;

	len = 0;
	save = 0;
	remove_escaped_char(*str_cmd, elem, i, count_escape);
	while ((*str_cmd)->is_in_dbquote == 1)
		scmd_move_to_next_char(*str_cmd);
	remove_escaped_char(*str_cmd, elem, i, count_escape);

	scmd_clean(*str_cmd);
	scmd_init(*str_cmd, elem->input[i]);

	*count_escape = 0;
	pos = 0;

	while (elem->input[i][pos] && scmd_cur_char(*str_cmd))
	{
		ft_printf("\n\ninput: |%s|\nstr_cmd: |%s|\n\n", &(elem->input[i][pos]), &((*str_cmd)->str[(*str_cmd)->pos]));
		if (scmd_cur_char_is_escaped(*str_cmd) == 1)
		{
			remove_escaped_char_db(elem, i, &pos);
			scmd_move_to_next_char(*str_cmd);
		}
		else if (scmd_cur_char(*str_cmd) == '$')
		{
			save = i;
			if (pos == 0)
				pos = (*str_cmd)->pos;
			len = expand_db(elem, alloc, &save, &pos);
			pos += len;
			while (scmd_cur_char(*str_cmd) != '\\'
					&& scmd_cur_char(*str_cmd) != ' '
					&& scmd_cur_char(*str_cmd))
			{
				scmd_move_to_next_char(*str_cmd);
				if (scmd_cur_char(*str_cmd) == '$')
					break ;
				ft_printf("strcmd: %s\n", scmd_cur_str(*str_cmd));
			}
		}
		else
		{
			scmd_move_to_next_char(*str_cmd);
			pos += 1;
		}
	}
}


int	inhibitor(t_ast *elem, t_alloc *alloc)
{
	t_str_cmd_inf	*str_cmd;
	int				i;
	int				save;
	size_t			pos_elem;

	i = 0;
	pos_elem = 0;
	str_cmd = NULL;
	if (!(str_cmd = (t_str_cmd_inf*)malloc(sizeof(t_str_cmd_inf))))
		ft_exit_malloc();
	while (elem->input[i])
	{
		save = 0;
		pos_elem = 0;
		if (!scmd_init(str_cmd, elem->input[i]))
			return (0);
		while (scmd_cur_char(str_cmd))
		{
			if (str_cmd->is_in_quote == 1)
				go_to_end_quote(str_cmd, elem, i, &pos_elem);
			else if (str_cmd->is_in_dbquote)
				inhib_in_dbquote(elem, i, &str_cmd, &pos_elem, alloc);
			else if (scmd_cur_char_is_escaped(str_cmd) == 1)
				remove_escaped_char(str_cmd, elem, i, &pos_elem);
			else if (scmd_cur_char(str_cmd) == '$')
			{
				save = i;
				expand(elem, alloc, &save, str_cmd->pos - pos_elem);
			}
			scmd_move_to_next_char(str_cmd);
			pos_elem += 1;
			// ft_printf("\n\nstrcmd: %s\ninput: %s\n", scmd_cur_str(str_cmd), elem->input[i]);
		}
		scmd_clean(str_cmd);
		if (save != 0)
			i = save;
		else
			i += 1;
	}
	return (1);
}
