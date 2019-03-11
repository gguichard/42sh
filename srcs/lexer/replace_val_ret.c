#include "../../includes/shell.h"
# include "../../includes/parser_lexer.h"

int	replace_val_ret(char **str, int i, int x)
{
	char	*value;

	ft_delete_inside(str, i, x);
	value = ft_itoa(ret_status());
	replace_str(str, value, i);
	ft_memdel((void **)&value);
	return (0);
}

int	ret_status(void)
{
	int		err;

	err = 0;
	if (!g_ret[1])
		return ((unsigned char)g_ret[0]);
	if (WIFEXITED(g_ret[0]))
		err = WEXITSTATUS(g_ret[0]);
	else if (WIFSIGNALED(g_ret[0]))
		err = WTERMSIG(g_ret[0]) + 128;
	else if (WIFSTOPPED(g_ret[0]))
		err = WSTOPSIG(g_ret[0]) + 128;
	g_ret[0] = err;
	g_ret[1] = 0;
	return (err);
}
