#include "shell.h"
#include "error.h"

void	exec_file_error(t_error err, char *files)
{
	const char	*s;

	s = error_to_str(err);
	if (err == ERRC_UNEXPECTED)
	{
		s = "malloc failed";
		g_ret[0] = 1;
	}
	ft_dprintf(2, "42sh: %s: %s\n", files, error_to_str(err));
	if (err == ERRC_CMDNOTFOUND)
		g_ret[0] = 127;
}
