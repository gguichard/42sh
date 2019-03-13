#include "libft.h"
#include "str_cmd_inf.h"

/*
** Deplace le curseur vers la droite sans gerer les cas ou il se trouve entre
** des quotes ou dans un nom de variable {}.
*/
//todo subcmd + relire pour verifier
static void		interpret_char_not_in_quote_or_var(t_str_cmd_inf *str_cmd_inf)
{
	t_str_cmd_inf *new_scmd;

	if (str_cmd_inf->str[str_cmd_inf->pos] == '\''
			&& !str_cmd_inf->is_in_doublequote)
		str_cmd_inf->is_in_quote = 1;
	else if (str_cmd_inf->str[str_cmd_inf->pos] == '\"')
		str_cmd_inf->is_in_doublequote = !str_cmd_inf->is_in_doublequote;
	else if (str_cmd_inf->str[str_cmd_inf->pos] == '{'
			&& str_cmd_inf->pos > 0
			&& str_cmd_inf->str[str_cmd_inf->pos - 1] == '$'
			&& !scmd_char_at_is_escaped(str_cmd_inf, str_cmd_inf->pos - 1))
	{
		str_cmd_inf->is_in_var_bracket = 1;
		if ((new_scmd = (t_str_cmd_inf*)malloc(sizeof(t_str_cmd_inf))) != NULL)
		{
			scmd_init(new_scmd, str_cmd_inf->str);
			new_scmd->pos = str_cmd_inf->pos + 1;
		}
		str_cmd_inf->sub_var_bracket = new_scmd;
	}
}

int				scmd_cur_char_is_in_nothing(t_str_cmd_inf *str_cmd_inf)
{
	return (!str_cmd_inf->is_in_quote
			&& !str_cmd_inf->is_in_doublequote
			&& !str_cmd_inf->is_in_subcmd
			&& !str_cmd_inf->is_in_var_bracket);
}

int				scmd_char_at_is_escaped(t_str_cmd_inf *str_cmd_inf
		, size_t at_pos)
{
	if (str_cmd_inf->is_in_quote)
		return (str_cmd_inf->str[str_cmd_inf->pos] != '\'');
	if (str_cmd_inf->is_in_doublequote
			&& ft_strchr(DOUBLEQUOTE_SPE_CHAR, str_cmd_inf->str[at_pos])
			== NULL)
		return (1);
	if (str_cmd_inf->is_in_subcmd
			&& ft_strchr(SUBCMD_SPE_CHAR, str_cmd_inf->str[at_pos])
			== NULL)
		return (1);
	return (at_pos > 0 && str_cmd_inf->str[at_pos - 1] == '\\'
			&& !scmd_char_at_is_escaped(str_cmd_inf, at_pos - 1));
}

//todo subcmd + relire pour verifier
static int		interpret_char_in_var_bracket(t_str_cmd_inf *str_cmd_inf)
{
	if (str_cmd_inf->sub_var_bracket != NULL)
	{
		if (!interpret_char_in_var_bracket(str_cmd_inf->sub_var_bracket))
		{
			ft_memdel((void**)&(str_cmd_inf->sub_var_bracket));
			str_cmd_inf->is_in_var_bracket = 0;
		}
		else
			++(str_cmd_inf->sub_var_bracket->pos);
		return (1);
	}
	if (!scmd_cur_char_is_escaped(str_cmd_inf))
	{
		if (str_cmd_inf->is_in_quote)
		{
			if (str_cmd_inf->str[str_cmd_inf->pos] == '\'')
				str_cmd_inf->is_in_quote = 0;
		}
		else if (!str_cmd_inf->is_in_dbquote && !str_cmd_inf->is_in_subcmd
				&& str_cmd_inf->str[str_cmd_inf->pos] == '}')
			return (0);
		else
			interpret_char_not_in_quote_or_var(str_cmd_inf);
	}
	return (1);
}

//todo subcmd + relire pour verifier
int				scmd_move_to_next_char(t_str_cmd_inf *str_cmd_inf)
{
	if (str_cmd_inf->str[str_cmd_inf->pos] == '\0')
		return (0);
	if (!scmd_cur_char_is_escaped(str_cmd_inf))
	{
		if (str_cmd_inf->is_in_quote)
		{
			if (str_cmd_inf->str[str_cmd_inf->pos] == '\'')
				str_cmd_inf->is_in_quote = 0;
		}
		else if (str_cmd_inf->is_in_var_bracket)
		{
			interpret_char_in_var_bracket(str_cmd_inf);
		}
		else
		{
			interpret_char_not_in_quote_or_var(str_cmd_inf);
		}
	}
	++(str_cmd_inf->pos);
	return (1);
}
