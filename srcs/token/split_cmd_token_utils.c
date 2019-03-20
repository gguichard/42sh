#include <stdlib.h>
#include "libft.h"
#include "str_cmd_inf.h"
#include "token_inf.h"
#include "split_cmd_token.h"

int				is_a_sep_char(char c)
{
	if (c == '\0')
		return (0);
	else
		return (ft_strchr(WORD_SEP_CHARS CMD_SEP_CHARS OPE_CHARS, c) != NULL);
}

int				cur_token_is_number(t_str_cmd_inf *str_cmd_inf
		, const char *token_start)
{
	while (token_start != str_cmd_inf->str + str_cmd_inf->pos)
	{
		if (!ft_isdigit(*token_start))
			return (0);
		++token_start;
	}
	return (1);
}

int				get_cur_token_len(const char *token_start)
{
	size_t	len;

	len = 0;
	if (is_a_sep_char(*token_start))
	{
		++len;
		if ((*token_start == '>' || *token_start == '<')
				&& *token_start == token_start[1])
			++len;
	}
	return (len);
}

t_token_type	get_token_for_opt_add(t_str_cmd_inf *str_cmd_inf
		, const char *token_start, int *end_by_and)
{
	if (str_cmd_inf->str[str_cmd_inf->pos] == '>'
			|| str_cmd_inf->str[str_cmd_inf->pos] == '<')
	{
		if (cur_token_is_number(str_cmd_inf, token_start))
			return (TK_LRED_OPT);
		else if (str_cmd_inf->pos > 0
				&& get_cur_token_len(str_cmd_inf->str + str_cmd_inf->pos) == 1
				&& str_cmd_inf->str[str_cmd_inf->pos] == '>'
				&& str_cmd_inf->str[str_cmd_inf->pos - 1] == '&')
		{
			*end_by_and = 1;
			return (TK_WORD);
		}
	}
	return (TK_WORD);
}
