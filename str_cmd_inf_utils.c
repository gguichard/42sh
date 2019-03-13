#include "libft.h"
#include "str_cmd_inf.h"

void	scmd_init(t_str_cmd_inf *str_cmd_inf, const char *str)
{
	ft_bzero(str_cmd_inf, sizeof(t_str_cmd_inf));
	str_cmd_inf->str = str;
}

void	scmd_delete(t_str_cmd_inf *str_cmd_inf)
{
	if (str_cmd_inf != NULL)
	{
		scmd_delete(str_cmd_inf->sub_var_bracket);
		free(str_cmd_inf);
	}
}

int		scmd_cur_char_is_escaped(t_str_cmd_inf *str_cmd_inf)
{
	return (scmd_char_at_is_escaped(str_cmd_inf, str_cmd_inf->pos));
}
