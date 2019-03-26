#include "shell.h"
#include "parser_lexer.h"

int				replace_val_ret(char **str, int i, int x, int err)
{
	char	*value;

	ft_delete_inside(str, i, x);
	value = ft_itoa(err);
	replace_str(str, value, i);
	ft_memdel((void **)&value);
	return (0);
}
