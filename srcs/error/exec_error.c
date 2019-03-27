#include "shell.h"
#include "error.h"

void	exec_file_error(t_error err, char *files)
{
	const char	*s;

	s = error_to_str(err);
	if (err == ERRC_UNEXPECTED)
		s = "malloc failed";
	ft_dprintf(2, "42sh: %s: %s\n", files, error_to_str(err));
}
