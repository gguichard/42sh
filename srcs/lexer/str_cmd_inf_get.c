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

int			scmd_cur_is_of(t_str_cmd_inf *str_cmd_inf, const char *these)
{
	if (scmd_cur_char(str_cmd_inf) == '\0')
		return (0);
	return (ft_strchr(these, scmd_cur_char(str_cmd_inf)) != NULL);
}
