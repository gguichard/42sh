#include "shell.h"

int	ft_fd_exist(char *str_fd)
{
	int		fd;
	int		fd_tmp;
	size_t	len;

	fd_tmp = -1;
	if (!str_fd)
		return (0);
	fd = ft_atoi(str_fd);
	if (fd >= 0 && fd <= 9)
	{
		close(fd_tmp);
		return (fd);
	}
	len = ft_strlen(str_fd);
	len = (str_fd[len - 1] == '-') ? len - 1 : len;
	write(2, "21sh: ", 6);
	write(2, str_fd, len);
	write(2, ": bad file descriptor\n", 22);
	return (-1);
}
