#include <stdlib.h>
#include "libft.h"
#include "str_cmd_inf.h"

char		scmd_cur_char(t_str_cmd_inf *str_cmd_inf)
{
	return (str_cmd_inf->str[str_cmd_inf->pos]);
}

const char	*scmd_cur_str(t_str_cmd_inf *str_cmd_inf)
{
	return (str_cmd_inf->str + str_cmd_inf->pos);
}

int			scmd_char_at_is_of(t_str_cmd_inf *str_cmd_inf, size_t pos
		, const char *these)
{
	if (str_cmd_inf->str[pos] == '\0')
		return (0);
	return (ft_strchr(these, str_cmd_inf->str[pos]) != NULL);
}

int			scmd_cur_is_of(t_str_cmd_inf *str_cmd_inf, const char *these)
{
	return (scmd_char_at_is_of(str_cmd_inf, str_cmd_inf->pos, these));
}
