#include <unistd.h>
#include "error.h"

void	exec_file_error(t_error err, const char *file)
{
	const char	*str;

	if (err == ERRC_FILENOTFOUND)
		err = ERRC_CMDNOTFOUND;
	if (err == ERRC_UNEXPECTED)
		str = "malloc failed";
	else
		str = error_to_str(err);
	ft_dprintf(STDERR_FILENO, "42sh: %s: %s\n", file, str);
}
