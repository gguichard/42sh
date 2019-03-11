#include <unistd.h>
#include <stdlib.h>
#include "get_next_line.h"
#include "printf.h"

//TODO faire un vrai main
int		main(int argc, char **argv)
{
	int		gnl_ret;
	char	*line;

	(void)argc;
	(void)argv;
	while ((gnl_ret = get_next_line(STDOUT_FILENO, &line)) > 0)
	{
		//parse line etc;
		free(line);
	}
	ft_printf("GNL ret : %d\n", gnl_ret);
	return (0);
}
