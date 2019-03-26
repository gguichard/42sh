#include "libft.h"
#include "str_cmd_inf.h"

static int		is_dollared_spe_char(t_str_cmd_inf *str_cmd_inf, size_t at_pos
		, const char *spe_char_lst)
{
	int		is_a_spe_char;

	if (at_pos <= 0 || str_cmd_inf->str[at_pos - 1] != '$'
			|| scmd_char_at_is_escaped(str_cmd_inf, at_pos - 1))
		return (0);
	is_a_spe_char = 0;
	while (*spe_char_lst)
	{
		if (*spe_char_lst == str_cmd_inf->str[at_pos])
		{
			is_a_spe_char = 1;
			break ;
		}
		++spe_char_lst;
	}
	return (is_a_spe_char);
}

int				scmd_char_at_is_escaped(t_str_cmd_inf *str_cmd_inf
		, size_t at_pos)
{
	while (str_cmd_inf->sub_str_cmd != NULL)
		str_cmd_inf = str_cmd_inf->sub_str_cmd;
	if (str_cmd_inf->is_in_quote)
		return (str_cmd_inf->str[str_cmd_inf->pos] != '\'');
	if (str_cmd_inf->is_in_dbquote
			&& !is_dollared_spe_char(str_cmd_inf, at_pos, "{(")
			&& ft_strchr(DBQUOTE_SPE_CHAR, str_cmd_inf->str[at_pos])
			== NULL)
		return (1);
	if (str_cmd_inf->cur_str_cmd_type == SCMD_TYPE_SUBCMD
			&& !is_dollared_spe_char(str_cmd_inf, at_pos, "{(")
			&& ft_strchr(SUBCMD_SPE_CHAR, str_cmd_inf->str[at_pos])
			== NULL)
		return (1);
	return (at_pos > 0 && str_cmd_inf->str[at_pos - 1] == '\\'
			&& !scmd_char_at_is_escaped(str_cmd_inf, at_pos - 1));
}
