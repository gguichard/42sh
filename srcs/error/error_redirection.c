#include "shell.h"

void	error_redir(char *file)
{
	write(2, "21sh: permission denied: ", 25);
	write(2, file, ft_strlen(file));
	write(2, "\n", 1);
}

void	error_access(char *file)
{
	write(2, "21sh: no such file or directory: ", 33);
	write(2, file, ft_strlen(file));
	write(2, "\n", 1);
}
