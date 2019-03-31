#include <unistd.h>
#include "error.h"

void	exec_file_error(t_error err, const char *file)
{
	const char	*str;

	str = error_to_str(err);
	if (err == ERRC_UNEXPECTED)
		str = "malloc failed";
	ft_dprintf(STDERR_FILENO, "42sh: %s: %s\n", file, str);
}
