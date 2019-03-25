#include "shell.h"
#include "parser_lexer.h"
#include "expand.h"
#include "inhibitor.h"
#include "str_cmd_inf.h"


void	remove_escaped_char(t_str_cmd_inf *str_cmd, t_ast *elem, int i,
		size_t *count_escape)
{
	size_t	len;
	size_t	pos;

	pos = str_cmd->pos - *count_escape;
	len = ft_strlen(&(elem->input[i][pos]));
	ft_memmove((void*)&(elem->input[i][pos - 1]),
		(const void*)&(elem->input[i][pos]), len);
	elem->input[i][pos + len - 1] = '\0';
	*count_escape += 1;
}

void	go_to_end_quote(t_str_cmd_inf *str_cmd, t_ast *elem, int i,
		size_t *count_escape)
{
	remove_escaped_char(str_cmd, elem, i, count_escape);
	while (str_cmd->is_in_quote == 1)
		scmd_move_to_next_char(str_cmd);
	remove_escaped_char(str_cmd, elem, i, count_escape);
}

int			expand_db(t_ast *elem, t_alloc *alloc, int *i, size_t pos)
{
	char		**new;
	char		*str;
	const char	*exp;
	int			x;

	new = NULL;
	exp = NULL;
	str = NULL;
	x = 0;
	// while ((exp = ft_strchr(&(elem->input[*i][pos]), '$')))
	// {
	exp = ft_strchr(&(elem->input[*i][pos]), '$');

		x = ft_strlen(elem->input[*i]) - ft_strlen(exp);
		if (expand_var(&str, alloc, exp, &(elem->input[*i])) == 0)
			return (0);
	// }
	// if (str)
	// 	create_new_input(elem, i, ft_strsplit(elem->input[*i], ' '));
	return (ft_strlen(get_expand_value(*(alloc->var), &(exp[1]), 0)));
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
	ft_printf("elem clean: %s\n", elem->input[i]);
	while (elem->input[i][len] && scmd_cur_char(*str_cmd))
	{
		ft_printf("char: %c\n", scmd_cur_char(*str_cmd));
		if (scmd_cur_char_is_escaped(*str_cmd) == 1)
		{
			remove_escaped_char(*str_cmd, elem, i, count_escape);
		}
		else if (scmd_cur_char(*str_cmd) == '$')
		{
			save = i;
			pos = (*str_cmd)->pos - *count_escape;
			ft_printf("pos = %d et input: |%s|\n", pos, elem->input[save]);
			// ft_printf("IS ESCAPE = %d\n", scmd_cur_char_is_escaped(*str_cmd));
			// len = expand_db(elem, alloc, &save, len);
			expand_db(elem, alloc, &save, pos);
			pos += 1;
		}
		scmd_move_to_next_char(*str_cmd);
		len = (*str_cmd)->pos - *count_escape;
	}
}


int	inhibitor(t_ast *elem, t_alloc *alloc)
{
	t_str_cmd_inf	*str_cmd;
	int				i;
	int				save;
	size_t			count_escape;

	i = 0;
	count_escape = 0;
	str_cmd = NULL;
	if (!(str_cmd = (t_str_cmd_inf*)malloc(sizeof(t_str_cmd_inf))))
		ft_exit_malloc();
	while (elem->input[i])
	{
		save = 0;
		count_escape = 0;
		if (!scmd_init(str_cmd, elem->input[i]))
			return (0);
		while (elem->input[i][str_cmd->pos - count_escape] && scmd_cur_char(str_cmd))
		{
			if (str_cmd->is_in_quote == 1)
				go_to_end_quote(str_cmd, elem, i, &count_escape);
			else if (str_cmd->is_in_dbquote)
			{
				inhib_in_dbquote(elem, i, &str_cmd, &count_escape, alloc);
				// ft_printf("str_in_db: %s\n", str_cmd->str);
			}
			else if (scmd_cur_char_is_escaped(str_cmd) == 1)
				remove_escaped_char(str_cmd, elem, i, &count_escape);
			else if (scmd_cur_char(str_cmd) == '$')
			{
				save = i;
				expand(elem, alloc, &save, str_cmd->pos - count_escape);
			}
			scmd_move_to_next_char(str_cmd);
		}
		scmd_clean(str_cmd);
		if (save != 0)
			i = save;
		else
			i += 1;
	}
	return (1);
}
