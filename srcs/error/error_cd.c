#include "shell.h"
#include "error.h"

int	check_arg_cd(t_ast *elem, int i)
{
	int	x;

	x = 0;
	while (elem->input[i + x])
		x += 1;
	if (x > 1)
		return (error_cd(0, 4));
	return (i);
}

int	error_cd(char *err, int type)
{
	if (type == 4)
	{
		write(2, "21sh: cd: too many arguments\n", 29);
		g_ret[0] = 2;
		return (-1);
	}
	if (type == 0)
		write(2, "cd: no such file or directory: ", 31);
	else if (type == 1)
		write(2, "cd: permission denied: ", 23);
	else if (type == 2)
		write(2, "cd: ", 4);
	else if (type == 3)
		write(2, "cd: not a directory: ", 21);
	write(2, err, ft_strlen(err));
	(type == 2) ? write(2, " not set", 8) : 0;
	write(2, "\n", 1);
	g_ret[0] = 1;
	return (-1);
}
